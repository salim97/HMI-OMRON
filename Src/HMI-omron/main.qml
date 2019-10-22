/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import QtQuick.Controls 2.0 as QQC2
import "qml"
import "qml/Style"
import QtQuick.Controls 2.0
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick 2.7
import Qt.labs.settings 1.0
import "qml/Reglages"
import "qml/Validation"
import "qml/Services"
import "qml/Defauts"

QQC2.ApplicationWindow {

    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Wearable")
    // gestion d'arret d'urgence pour tous les pages
    Component.onCompleted: {
        myOMRON.c20400_read() // read value c204.00 from omron
        myOMRON.readALL() // to read all variables from omron
    }

    Connections{
        target: myOMRON

        onD1Changed:{
            //spinbox.value = myOMRON.d1 // update value to UI
            //??????????
        }
    }


    // background with all defaults
    background: Rectangle {
        id: rectangle
        x: 0
        y: 1
        width: 800
        height: 480
        color: "#c0c0c0"
        visible: true

        Image {
            id: image
            x: 0
            y: 347
            width: 250
            height: 133
            source: "images/CDA LCD 400.png"
        }
        Image {
            id: aru
            x: 535
            y: 369
            width: 265
            height: 111
            visible: true
            source: "../../images/aru.png"
        }

        Image {
            id: warning_rep
            x: 535
            y: 369
            width: 265
            height: 111
            visible: false
            source: "images/warning_rep.png"
        }

        Image {
            id: warning_ech
            x: 535
            y: 369
            width: 265
            height: 111
            visible: false
            source: "images/warning_ech.png"
        }
    }

    header: NaviButton {
        id: homeButton
        edge: Qt.TopEdge
        enabled: stackView.depth > 1
        imageSource: "images/home.png"
        //onClicked: stackView.pop()
        onPressed:  stackView.pop()
    }

    footer: NaviButton {
        id: backButton
        edge: Qt.BottomEdge
        enabled: stackView.depth > 1
        imageSource: "images/back.png"
        onClicked: stackView.pop()
    }

    QQC2.StackView {
        id: stackView
        visible: true
        focus: true
        anchors.fill: parent
        initialItem: LauncherPage {
            onLaunched: stackView.push(page)
        }
    }
}


/*
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
*/
