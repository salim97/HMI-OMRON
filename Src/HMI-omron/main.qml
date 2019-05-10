import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Item {
        anchors.fill: parent

        SpinBox{
            anchors.centerIn: parent
            from: 0
            to: 999
            stepSize: 1
            onValueChanged: myOMRON.d1 = value
        }
    }
}
