
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick 2.7
import Qt.labs.settings 1.0
import "../Style"

Item {
    width: 800


    SwipeView {
        id: svSettingsContainer

        anchors.fill: parent


        Item {
            id: settingsPage1
            visible: true



            Switch {
                id: switchEtiq
                x: 525
                y: 83
                text: qsTr("Etiq")
                hoverEnabled: true
                enabled: true
                property bool test: switchEtiq.checked
                checked: test


                onCheckedChanged:
                if(test==false)
                { myOMRON.h0 = Math.pow(2,6)
                    myOMRON.h0_send()}
                else
                { myOMRON.h0 = 0
                    myOMRON.h0_send()}

            }


            Switch {
                id: switchContreEtiq
                x: 525
                y: 173
                text: qsTr("ContreEtiq")
                hoverEnabled: true
                enabled: true
                //property bool checkeContreEtiq: switchContreEtiq.checked
                //checked: checkeContreEtiq == true ? grafcet.HContreEtiq =1 : grafcet.HContreEtiq=0
                checked: grafcet.HContreEtiq
                onCheckedChanged: grafcet.HContreEtiq = checked
            }


            Switch {
                id: switchEtiqContreEtiq
                x: 525
                y: 263
                text: qsTr("EtiqContreEtiq")
                hoverEnabled: true
                enabled: true
                //property bool checkeEtiqContreEtiq: switchEtiqContreEtiq.checked
                //checked: checkeEtiqContreEtiq == true ? grafcet.HEtiqContreEtiq =1 : grafcet.HEtiqContreEtiq=0
                checked:  grafcet.HEtiqContreEtiq
                onCheckedChanged: grafcet.HEtiqContreEtiq = checked
            }

            Text {
                id: etiquette
                x: 165
                y: 85
                text: qsTr("ETIQUETTE")
                visible: true
                font.pixelSize: 30
            }

            Text {
                id: contreetiquette
                x: 165
                y: 175
                text: qsTr("CONTRE ETIQUETTE")
                font.pixelSize: 30
            }

            Text {
                id: etiqContreEtiq
                x: 165
                y: 265
                text: qsTr("ETIQ CONTRE ETIQ")
                font.pixelSize: 30
            }

//            Rectangle{
//                 id:etiq
//                 x: 770
//                 y: 88
//                 width: 30
//                 height: 30
//                 color: grafcet.HEtiq == true ? "green" : "red"
//             }
//            Rectangle{
//                 id:ontreEtiq
//                 x: 770
//                 y: 178
//                 width: 30
//                 height: 30
//                 color: grafcet.HContreEtiq == true ? "green" : "red"
//             }
//            Rectangle{
//                 id:tiqContreEtiq
//                 x: 770
//                 y: 268
//                 width: 30
//                 height: 30
//                 color: grafcet.HEtiqContreEtiq == true ? "green" : "red"
//             }




        }

        Item {
            id: settingsPage2
            visible: true

            Switch {
                id: switchRepEtiq
                x: 525
                y: 83
                text: qsTr("RepEtiq")
                visible: true
                hoverEnabled: true
                enabled: true
                //property bool checkeRepEtiq: switchRepEtiq.checked
                //checked: checkeRepEtiq == true ? grafcet.HRepEtiq =1 : grafcet.HRepEtiq=0
                checked:  grafcet.HRepEtiq
                onCheckedChanged: grafcet.HRepEtiq = checked
            }



            Switch {
                id: switchRepCran
                x: 525
                y: 173
                text: qsTr("RepCran")
                hoverEnabled: true
                enabled: true
                //property bool checkeRepCran: switchRepCran.checked
                //checked: checkeRepCran == true ? grafcet.HRepCran =1 : grafcet.HRepCran=0
                checked: grafcet.HRepCran ;
                onCheckedChanged: grafcet.HRepCran = checked
            }





            Text {
                id: epEtiq
                x: 165
                y: 85
                text: qsTr("REPERAGE ETIQUETTE")
                visible: true
                font.pixelSize: 30
            }

            Text {
                id: repCran
                x: 165
                y: 175
                text: qsTr("REPERAGE CRAN")
                font.pixelSize: 30
            }

//            Rectangle{
//                 id:repEtiq
//                 x: 770
//                 y: 88
//                 width: 30
//                 height: 30
//                 color: grafcet.HRepEtiq == true ? "green" : "red"
//             }
//            Rectangle{
//                 id:epCran
//                 x: 770
//                 y: 178
//                 width: 30
//                 height: 30
//                 color: grafcet.HRepCran == true ? "green" : "red"
//             }


        }


   }


    PageIndicator {
        y: 412
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 48
        count: svSettingsContainer.count
        currentIndex: svSettingsContainer.currentIndex

        anchors.bottom: svSettingsContainer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
