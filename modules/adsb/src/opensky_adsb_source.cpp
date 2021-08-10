#include "opensky_adsb_source.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

// API: https://opensky-network.org/apidoc/rest.html

namespace
{
constexpr char baseUrl[] = "https://opensky-network.org/api";
constexpr char states[] = "states";
} // namespace

using namespace dreka::domain;

OpenskyAdsbSource::OpenskyAdsbSource(QObject* parent) : IAdsbSource(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &OpenskyAdsbSource::onFinished);
}

QJsonArray OpenskyAdsbSource::adsbData() const
{
    return m_adsbData;
}

void OpenskyAdsbSource::start()
{
    qDebug() << "start";
    m_started = true;
    this->get("/states/all");
}

void OpenskyAdsbSource::stop()
{
    if (m_lastReply)
    {
        m_lastReply->abort();
        m_lastReply->deleteLater();
    }

    m_started = false;
}

void OpenskyAdsbSource::get(const QString& request)
{
    m_lastReply = m_manager.get(QNetworkRequest(QNetworkRequest(QUrl(baseUrl + request))));
}

void OpenskyAdsbSource::onFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        m_adsbData = doc.object().value(::states).toArray();
        qDebug() << m_adsbData;
        emit adsbDataReceived(m_adsbData);
    }
    else
    {
        qWarning() << reply->errorString();
    }

    reply->deleteLater();

    if (m_started && reply == m_lastReply)
        this->start();
}