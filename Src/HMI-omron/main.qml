import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Connections{
        target: myOMRON

        onD1Changed:{
            spinbox.value = myOMRON.d1
            oldvalue = myOMRON.d1
        }
    }

    Item {
        anchors.fill: parent

        SpinBox{
            id: spinbox
            anchors.centerIn: parent
            from: 0
            to: 999
            stepSize: 1
            property int oldvalue: 0
            onValueChanged:
            {
                myOMRON.d1 = value
                value = oldvalue
            }
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

                onPressed: {myOMRON.h200 =1
                    manu_marche.color= "green"}
                onReleased: {myOMRON.h200 =0
                    manu_marche.color= "red"}
            }

        }
    }
}
