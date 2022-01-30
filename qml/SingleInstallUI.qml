import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

Item {
    id: root
    width: 700; height: 500

    Rectangle{
        id: bg
        width: 700; height: 500
        color: "#F8F8F8"

        Rectangle {
            id: sideFrame
            color: "#FFFFFF"
            width: 220
            height: 500
            anchors.left: bg.left
            anchors.top: bg.top
            Image {
                id: pkgIcon
                source: "qrc:/icon_install_light_resized.svg"
                width: 100
                height: 100
                antialiasing: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 80
            }
            Label {
                id: rpmName
                text: "RPM Name"
                font.pixelSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: pkgIcon.bottom
                anchors.topMargin: 20
            }
            Label {
                id: versionRelease
                text: "1.0.0-1.up1"
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: rpmName.bottom
            }
        }

        Rectangle {
            id: detailFrame
            color: "#FFFFFF"
            width: 464
            height: 484
            anchors.left: sideFrame.right
            anchors.top: sideFrame.top
            anchors.leftMargin: 8
            anchors.topMargin: 8
            radius: 8
            ScrollView {
                id: detailScrollView
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            }
        }

    }

}
