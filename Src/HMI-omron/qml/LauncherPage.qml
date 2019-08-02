

import QtQuick 2.8
import QtQuick.Controls 2.1 as QQC2
import "Style"
import QtQuick.Controls 2.0



PathView {
    id: circularView

    signal launched(string page)

    readonly property int cX: width / 2
    readonly property int cY: height / 2
    readonly property int itemSize: size / 4
    readonly property int size: Math.min(width - 80, height)
    readonly property int radius: size / 2 - itemSize / 3
    width: 800




    // gestion d'arret d'urgence

    //  compteur
    Rectangle{
        x: 281
        y: 384
        width: 237
        height: 42

        Label {
        id: co1
        x: 56
        y: 5
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 20


    }
    Text {
        id: text1
        x: 8
        y: 12
        text: qsTr("CMP:")
        font.bold: true
        font.pixelSize: 17
    }
    MouseArea{

        anchors.fill: parent

    }
}

// gestion marche / arret

    MouseArea{
        id: stopp
        width: 115
        height: 115
        visible: true
        x: 30
        y: 0

    Image {
        id: stop
        visible: true
        width: 100
        height: 100
        source: "../images/Reload0.png"
    }
    Text {
        x: 22
        y: 41

        text: qsTr("START")
        font.bold: true
    }
        }

    MouseArea{
        id: power
        x: 30
        y: 0
        width: 115
        height: 115
        visible: false



            AnimatedImage {
                id : run
                visible: true
                width: 100
                height: 100
                source: "../images/Reload.gif"
            }
            Text {
                x: 26
                y: 41

                text: qsTr("STOP")
                font.bold: true
            }

        }



// button shudown Pi

    Image {
        id: off
        x: 720
        y: 0
        width: 80
        height: 80
        source: "../images/off_80.png"

        MouseArea{
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            x: 710
            y: 0
            width: 90
            height: 90
            enabled: true
            //onClicked: {myGPIO.shutdown()}
        }


}




    // gestion defaut ( secu reperage / echenlliage )

        MouseArea{
        id:click_warning_rep

         x: 535
            y: 315
            width: 265
            height: 111
            visible: false
            enabled: true
        }

        MouseArea{
        id:click_warning_ech

         x: 535
            y: 315
            width: 265
            height: 111
            visible: false
            enabled: true
        }



    snapMode: PathView.SnapToItem

    model: ListModel {
        ListElement {
            title: qsTr("Validation")
            icon: "validation"
            page: "Validation/ValidationPage.qml"
        }
        ListElement {
            title: qsTr("Languages")
            icon: "language"
            page: "Language/LanguagePage.qml"
        }
        ListElement {
            title: qsTr("Help")
            icon: "info"
            page: "Help/HelpPage.qml"
        }
        ListElement {
            title: qsTr("Manuel")
            icon: "Compteur"
            page: "Compteurs/CompteurPage.qml"
        }
        ListElement {
            title: qsTr("Contact")
            icon: "notifications"
            page: "Contacts/ContactPage.qml"
        }
        ListElement {
            title: qsTr("Reglages")
            icon: "worldclock"
            page: "Reglages/ReglagePage.qml"
        }
        ListElement {
            title: qsTr("Service")
            icon: "settings"
            page: "Services/ServicesPage.qml"
        }
    }

    delegate: QQC2.RoundButton {
        width: circularView.itemSize
        height: circularView.itemSize

        property string title: model.title

        icon.width: 36
        icon.height: 36
        icon.name: model.icon
        opacity: PathView.itemOpacity
        padding: 12

        background: Rectangle {
            radius: width / 2
            border.width: 3
            //border.color: parent.PathView.isCurrentItem ? UIStyle.colorQtPrimGreen : UIStyle.colorQtGray4
            border.color: "blue"

        }

        onClicked: {
            if (PathView.isCurrentItem)
                circularView.launched(Qt.resolvedUrl(page))
            else
                circularView.currentIndex = index
        }
    }

    path: Path {
        startX: circularView.cX
        startY: circularView.cY
        PathAttribute {
            name: "itemOpacity"
            value: 1.0
        }
        PathLine {
            x: circularView.cX + circularView.radius
            y: circularView.cY
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.7
        }
        PathArc {
            x: circularView.cX - circularView.radius
            y: circularView.cY
            radiusX: circularView.radius
            radiusY: circularView.radius
            useLargeArc: true
            direction: PathArc.Clockwise
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.5
        }
        PathArc {
            x: circularView.cX + circularView.radius
            y: circularView.cY
            radiusX: circularView.radius
            radiusY: circularView.radius
            useLargeArc: true
            direction: PathArc.Clockwise
        }
        PathAttribute {
            name: "itemOpacity"
            value: 0.3
        }
    }

    Text {
        id: appTitle

        property Item currentItem: circularView.currentItem

        visible: currentItem ? currentItem.PathView.itemOpacity === 1.0 : 0

        text: currentItem ? currentItem.title : ""
        anchors.centerIn: parent
        anchors.verticalCenterOffset: (circularView.itemSize + height) / 2

        font.bold: true
        font.pixelSize: circularView.itemSize / 3
        font.letterSpacing: 1
        color: UIStyle.colorQtGray1
    }
}
