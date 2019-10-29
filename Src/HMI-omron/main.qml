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

//import QtQuick 2.7
//import QtQuick.Controls 2.0 as QQC2
//import "qml"
//import "qml/Style"
//import QtQuick.Controls 2.0
//import QtQuick 2.9
//import QtQuick.Window 2.2
//import QtQuick.Controls 2.0
//import QtQuick 2.7
//import Qt.labs.settings 1.0
//import "qml/Reglages"
//import "qml/Validation"
//import "qml/Services"
//import "qml/Defauts"

//QQC2.ApplicationWindow {

//    id: window
//    visible: true
//    width: 800
//    height: 480
//    title: qsTr("Wearable")
//    // gestion d'arret d'urgence pour tous les pages
//    Connections{
//        target: myOMRON

//        onD1Changed:{
//            spinbox.value = myOMRON.d1
//            //??????????

//        }
//    }

//    SpinBox{
//        id: spinbox
//        anchors.verticalCenterOffset: -89
//        anchors.horizontalCenterOffset: 287
//        anchors.centerIn: parent
//        from: 0
//            to: 999
//            stepSize: 1
//            onValueChanged:
//            {
//                myOMRON.d1 = value
//                myOMRON.d1_send()

//            }
//        }
//    Button{
//        anchors.top: spinbox.bottom
//        anchors.topMargin: 102
//        anchors.horizontalCenter: spinbox.horizontalCenter
//        text: "refresh"
//        anchors.horizontalCenterOffset: 20
//        onClicked: myOMRON.d1_read()
//    }


//    // background with all defaults
//    background: Rectangle {
//        id: rectangle
//        x: 0
//        y: 1
//        width: 800
//        height: 480
//        color: "#c0c0c0"
//        visible: true

//        Image {
//            id: image
//            x: 0
//            y: 347
//            width: 250
//            height: 133
//            source: "images/CDA LCD 400.png"
//        }
//        Image {
//            id: aru
//            x: 535
//            y: 369
//            width: 265
//            height: 111
//            visible: true
//            source: "../../images/aru.png"
//        }

//        Image {
//            id: warning_rep
//            x: 535
//            y: 369
//            width: 265
//            height: 111
//            visible: false
//            source: "images/warning_rep.png"
//        }

//        Image {
//            id: warning_ech
//            x: 535
//            y: 369
//            width: 265
//            height: 111
//            visible: false
//            source: "images/warning_ech.png"
//        }
//    }

//    header: NaviButton {
//        id: homeButton
//        edge: Qt.TopEdge
//        enabled: stackView.depth > 1
//        imageSource: "images/home.png"
//        //onClicked: stackView.pop()
//        onPressed:  stackView.pop()
//    }

//    footer: NaviButton {
//        id: backButton
//        edge: Qt.BottomEdge
//        enabled: stackView.depth > 1
//        imageSource: "images/back.png"
//        onClicked: stackView.pop()
//    }

//    QQC2.StackView {
//        id: stackView
//        visible: true
//        focus: true
//        anchors.fill: parent
//        initialItem: LauncherPage {
//            onLaunched: stackView.push(page)
//        }
//    }
//}


import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Component.onCompleted: {
        myOMRON.readALL() // to read all variables from omron

    }

    Connections{
        target: myOMRON

        onD1Changed:{
            console.log("========================== onD1Changed ==============================");
            console.log(myOMRON.d1)
            spinbox.value = myOMRON.d1
        }

        onH0Changed:{
//            line 215 et 216 kifkif, c'est une facone d'ecrire, math.pow(2,2) hiya position 2 donc h0_toBIN_asBOOL()[2] soit true soit false ,donc 1 wla 0
            //if(myOMRON.h0_toBIN_asBOOL()[2] == Math.pow(2,2))
            console.log("=========================== onH0Changed =============================");
            console.log(myOMRON.h0_toBIN_asSTRING())
            if(myOMRON.h0_toBIN_asBOOL()[2])
                manu_marche.color= "green"
            else
                manu_marche.color= "red"
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
            text: "refresh D1"
            onClicked:
            {
                myOMRON.h0_read()
                myOMRON.d1_read()
            }
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

            onPressed: {
                myOMRON.h0= Math.pow(2,2)
                myOMRON.h0_send()
                manu_marche.color= "green"
            }
            onReleased: {
                myOMRON.h0 =0
                myOMRON.h0_send()
                manu_marche.color= "red"
            }

        }

    }


}
