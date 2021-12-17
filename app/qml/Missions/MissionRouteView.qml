import QtQuick 2.6
import QtQuick.Layouts 1.12
import Industrial.Controls 1.0 as Controls
import Dreka 1.0

Row {
    id: root

    readonly property real availableWidth: width - missionButton.width - wpBox.width - spacing * 2

    property alias vehicleId : missionRouteController.vehicleId

    spacing: 1

    MissionRouteController { id: missionRouteController }

    Controls.MenuItem {
        id: navToItem
        enabled: controller.selectedVehicle !== undefined // TODO: online
        text: qsTr("Nav to")
        onTriggered: {
            controller.sendCommand("setMode", [ "NavTo" ]); // FIXME: to domain, packed commands
            missionRouteController.navTo(mapMenu.latitude, mapMenu.longitude);
        }
    }

    Component.onCompleted: {
        map.registerController("missionRouteController", missionRouteController);
        mapMenu.addItem(navToItem);
    }

    Controls.Button {
        id: missionButton
        height: parent.height
        flat: true
        rightCropped: true
        iconSource: "qrc:/icons/route.svg"
        tipText: qsTr("Mission")
        highlighted: missionPopup.visible
        enabled: controller.selectedVehicle !== undefined
        onClicked: missionPopup.visible ? missionPopup.close() : missionPopup.open()

        MissionOperationView {
            id: missionPopup
            x: -width - Controls.Theme.margins - Controls.Theme.spacing
            y: parent.y - height + parent.height
            closePolicy: Controls.Popup.CloseOnPressOutsideParent
            missionId: missionRouteController.mission.id
        }
    }

    Controls.ComboBox {
        id: wpBox
        width: root.width / 2.5
        flat: true
        labelText: qsTr("WPT")
        enabled: controller.selectedVehicle !== undefined
        model: missionRouteController.routeItems
        displayText: missionRouteController.routeItems[missionRouteController.currentItem]
        Binding on currentIndex {
            value: missionRouteController.currentItem
            when: !wpBox.activeFocus
        }
        onActivated: missionRouteController.switchItem(index)
    }
}
