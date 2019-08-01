import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Component.onCompleted: {
        myOMRON.c20400_read() // read value c204.00 from omron
        myOMRON.readALL() // to read all variables from omron
    }

    Connections{
        target: myOMRON

        onD1Changed:{
            spinbox.value = myOMRON.d1
            //??????????
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
            onValueChanged:
            {
                myOMRON.d1 = value
                myOMRON.d1_send()
                myOMRON.sendALL()
            }
        }
        Button{
            anchors.top: spinbox.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: spinbox.horizontalCenter
            text: "refresh"
            onClicked: myOMRON.d1_read()
        }




    }

    Rectangle {
        id: manu_marche
        x: 76
        y: 30
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

            onPressed: {myOMRON.c20000 = 1
                myOMRON.c20000_send()
                manu_marche.color= "green"}
            onReleased: {myOMRON.c20000 =0
                myOMRON.c20000_send()
                manu_marche.color= "red"}

        }

    }

    Rectangle {
        id: manu_marche1
        x: 76
        y: 98
        width: 65
        height: 62
        color: "#ff0000"
        radius: 50
        border.color: "#000000"
        MouseArea {
            id: b1
            anchors.fill: parent
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            onPressed: {myOMRON.c20100 = 1
                myOMRON.c20100_send()
                manu_marche1.color= "green"}
            onReleased: {myOMRON.c20100 =0
                myOMRON.c20100_send()
                manu_marche1.color= "red"}
        }
        border.width: 5
    }

    Rectangle {
        id: manu_marche2
        x: 76
        y: 172
        width: 65
        height: 62
        color: "#ff0000"
        radius: 50
        border.color: "#000000"
        MouseArea {
            anchors.fill: parent
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            onPressed: {myOMRON.c20200 = 1
                myOMRON.c20200_send()
                manu_marche2.color= "green"}
            onReleased: {myOMRON.c20200 =0
                myOMRON.c20200_send()
                manu_marche2.color= "red"}
        }
        border.width: 5
    }

    Rectangle {
        id: manu_marche3
        x: 76
        y: 245
        width: 65
        height: 62
        color: "#ff0000"
        radius: 50
        border.color: "#000000"
        MouseArea {
            anchors.fill: parent
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            onPressed: {myOMRON.c20300 = 1
                myOMRON.c20300_send()
                manu_marche3.color= "green"}
            onReleased: {myOMRON.c20300 =0
                myOMRON.c20300_send()
                manu_marche3.color= "red"}
        }
        border.width: 5
    }

    Rectangle {
        id: manu_marche4
        x: 76
        y: 318
        width: 65
        height: 62
        color: "#ff0000"
        radius: 50
        border.color: "#000000"
        MouseArea {
            anchors.fill: parent
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            onPressed: {myOMRON.c20400 = true
                myOMRON.c20400_send()
                manu_marche4.color= "green"}
            onReleased: {myOMRON.c20400 =false
                myOMRON.c20400_send()
                manu_marche4.color= "red"}
        }
        border.width: 5
    }

    Rectangle {
        id: manu_marche5
        x: 76
        y: 396
        width: 65
        height: 62
        color: "#ff0000"
        radius: 50
        border.color: "#000000"
        MouseArea {
            anchors.fill: parent
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            onPressed: {myOMRON.c20500 = 1
                myOMRON.c20500_send()
                manu_marche5.color= "green"}
            onReleased: {myOMRON.c20500 =0
                myOMRON.c20500_send()
                manu_marche5.color= "red"}
        }
        border.width: 5
    }
}
