#ifndef MISSION_CONTROLLER_H
#define MISSION_CONTROLLER_H

#include "i_missions_repository.h"
#include "i_property_tree.h"

namespace md::presentation
{
class MissionController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant vehicleId WRITE setVehicleId)
    Q_PROPERTY(QJsonObject mission READ mission NOTIFY missionChanged)
    Q_PROPERTY(QJsonObject operation READ operation NOTIFY operationChanged)
    Q_PROPERTY(QStringList waypoints READ waypoints NOTIFY waypointsChanged)
    Q_PROPERTY(int currentWaypoint READ currentWaypoint NOTIFY currentWaypointChanged)

public:
    explicit MissionController(QObject* parent = nullptr);

    QJsonObject mission() const;
    QJsonObject operation() const;
    QStringList waypoints() const;
    int currentWaypoint() const;

public slots:
    void setVehicleId(const QVariant& vehicleId);
    void setMission(domain::Mission* mission);

    void save(const QJsonObject& data);
    void remove();
    void upload();
    void download();
    void clear();
    void cancel();
    void switchWaypoint(int index);

signals:
    void missionChanged();
    void operationChanged();
    void waypointsChanged();
    void currentWaypointChanged();

private:
    domain::IMissionsRepository* const m_missionsRepository;
    domain::Mission* m_mission = nullptr;
};
} // namespace md::presentation

#endif // MISSION_CONTROLLER_H