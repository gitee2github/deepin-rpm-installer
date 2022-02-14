import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.11

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
                anchors.topMargin: 60
            }
            Label {
                id: rpmName
                text: rpmInfo.name
                font.pixelSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: pkgIcon.bottom
                anchors.topMargin: 15
            }
            Label {
                id: versionRelease
                text: rpmInfo.versionRelease
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: rpmName.bottom
            }
            Label {
                id: summary
                text: rpmInfo.summary
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: versionRelease.bottom
                anchors.topMargin: 30
                wrapMode: "WordWrap"
                width: 180
                anchors.leftMargin: 20
            }
            Button {
                text: "Install"
                width: 160
                height: 30
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: installPage.startPkgOperation("install")
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
                contentWidth: parent.width
                contentHeight: archTitle.height+arch.height+licenseTitle.height+license.height+descriptionTitle.height+description.height+requiresTitle.height+requires.height+providesTitle.height+provides.height+165
                clip: true
                anchors.fill: parent
                anchors.left: parent.left
                anchors.leftMargin: 30
                Label {
                    id: archTitle
                    text: "Architecture"
                    font.bold: true
                    anchors.top: parent.top
                    anchors.topMargin: 30
                }
                Label {
                    id: arch
                    text: rpmInfo.arch
                    anchors.top: archTitle.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
                Label {
                    id: licenseTitle
                    text: "License"
                    font.bold: true
                    anchors.top: arch.bottom
                    anchors.topMargin: 20
                }
                Label {
                    id: license
                    text: rpmInfo.license
                    anchors.top: licenseTitle.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
                Label {
                    id: descriptionTitle
                    text: "Description"
                    font.bold: true
                    anchors.top: license.bottom
                    anchors.topMargin: 20
                }
                Label {
                    id: description
                    text: rpmInfo.description
                    wrapMode: "WordWrap"
                    width: 400
                    anchors.top: descriptionTitle.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
                Label {
                    id: requiresTitle
                    text: "Requires"
                    font.bold: true
                    anchors.top: description.bottom
                    anchors.topMargin: 20
                }
                Label {
                    id: requires
                    text: rpmInfo.pkgRequiresStr
                    anchors.top: requiresTitle.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
                Label {
                    id: providesTitle
                    text: "Provides"
                    font.bold: true
                    height: 20
                    anchors.top: requires.bottom
                    anchors.topMargin: 20
                }
                Label {
                    id: provides
                    text: rpmInfo.pkgProvidesStr
                    anchors.top: providesTitle.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                }
            }
        }

    }

}
