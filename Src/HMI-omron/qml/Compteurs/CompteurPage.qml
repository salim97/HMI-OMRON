
import QtQuick 2.7
import QtQuick.Controls 2.0 as QQ2
//import "../Style"
import QtQuick.Controls 1.4



Item {


    width: 800





    // les manueles

    Text {
        id: text41
        x: 317
        y: 8
        text: qsTr("MANUEL")
        font.underline: true
        font.bold: true
        font.pixelSize: 35
    }

    Rectangle {
        id: manu_marche
        x: 263
        y: 74
        width: 65
        height: 62
        radius:  50
        color: "red"
        border.color: "black"
        border.width: 5
        MouseArea{
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

        }

    }
    Rectangle {
        id: manu_moteur1
        x: 263
        y: 171
        width: 65
        height: 62
        radius:  50
        color: "red"
        border.color: "black"
        border.width: 5
        MouseArea{
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
        }

    }



    Rectangle {
        id: manu_moteur2
        x: 639
        y: 74
        width: 65
        height: 62
        radius:  50
        color: "red"
        border.color: "black"
        border.width: 5
        MouseArea{
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
        }

    }
    Rectangle {
        id: manu_lisseur
        x: 639
        y: 171
        width: 65
        height: 62
        radius:  50
        color: "red"
        border.color: "black"
        border.width: 5
        MouseArea{
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
        }

    }


    Text {
        id: text1
        x: 56
        y: 90
        text: qsTr("MARCHE")
        font.pixelSize: 25
    }

    Text {
        id: text2
        x: 56
        y: 187
        text: qsTr("POSTE 1")
        font.pixelSize: 25
    }
    Text {
        id: text3
        x: 410
        y: 90
        text: qsTr("POSTE 2")
        font.pixelSize: 25
    }

    Text {
        id: text4
        x: 410
        y: 187
        text: qsTr("LISSEUR")
        font.pixelSize: 25
    }


    // lecture des sortie écran

//        Rectangle{
//             id:marche
//             x: 762
//             y: 200
//             width: 30
//             height: 30
//             color: myGPIO.marche == true ? "green" : "red"
//         }

//        Rectangle{
//             id:moteur1
//             x: 762
//             y: 250
//             width: 30
//             height: 30
//             color: myGPIO.moteur1 == true ? "green" : "red"
//         }

//        Rectangle{
//             id:moteur2
//             x: 762
//             y: 300
//             width: 30
//             height: 30
//             color: myGPIO.moteur2 == true ? "green" : "red"
//         }

//        Rectangle{
//             id:lisseur
//             x: 762
//             y: 350
//             width: 30
//             height: 30
//             color: myGPIO.lisseur == true ? "green" : "red"

//         }


//        // compteur

//        Label {
//            id: co1
//            x: 339
//            y: 256
//            text: grafcet.C1
//            horizontalAlignment: Text.AlignHCenter
//            font.pointSize: 20
//        }

//        Rectangle {
//            id: c1
//            x: 558
//            y: 241
//            width: 65
//            height: 62
//            radius:  50
//            color: "#0eff00"
//            border.color: "black"
//            border.width: 5
//            MouseArea{
//                anchors.rightMargin: 0
//                anchors.bottomMargin: 0
//                anchors.leftMargin: 0
//                anchors.topMargin: 0
//                anchors.fill: parent
//                //onClicked: grafcet.C1=0
//                onDoubleClicked: grafcet.C1=0

//            }
//            Text {
//                x: 11
//                y: 20

//                text: qsTr("RAZ")
//                font.pointSize: 14
//                font.bold: true
//            }

//        }

//        Text {
//            id: text42
//            x: 98
//            y: 251
//            text: qsTr("COMPTEUR")
//            font.underline: true
//            font.pixelSize: 35
//            font.bold: true

//        }

}

