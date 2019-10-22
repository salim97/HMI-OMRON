

import QtQuick 2.7
import QtQuick.Controls 2.0 as QQC2
import "Style"

QQC2.AbstractButton {

    id: button



    property int edge: Qt.TopEdge
    property alias imageSource: image.source

    contentItem: Image {
        id: image
        fillMode: Image.Pad
        sourceSize { width: 50; height: 50 } // ### TODO: resize the image
    }

    background: Rectangle {
        y: 40
        height: 250 //button.height * 4
        width:  250 //height
        radius: 125 //width / 2

        anchors.horizontalCenter: button.horizontalCenter
        anchors.top: edge === Qt.BottomEdge ? button.top : undefined
        anchors.bottom: edge === Qt.TopEdge ? button.bottom : undefined

        color: UIStyle.colorQtGray2
    }

    transform: Translate {
        Behavior on y { NumberAnimation { } }
        y: enabled ? 0 : edge === Qt.TopEdge ? -button.height : button.height
    }
}
