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
//import "../Style"
import QtQuick.Controls 2.0

import Qt.labs.settings 1.0
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick 2.7
import Qt.labs.settings 1.0



Item {

    width: 800
    height: 480




    SwipeView {
        id: svSettingsContainer

        anchors.fill: parent

        Item{
            width: 800
            height: 480
            id :etiquette1
            visible: true


            Text {
                id: text41
                x: 293
                y: 41
                text: qsTr("ETIQUETTE")
                font.underline: true
                font.bold: true
                font.pixelSize: 35
            }


        SpinBox {
            id: retard_etiq1
            x: 333
            y: 132
            width: 135
            height: 61      
            from: 0
            value: grafcet.D4
            to: 5000
            stepSize: 5
            onValueChanged:
                grafcet.D4 = retard_etiq1.value
        }

        SpinBox {
            id: arret_etiqE
            x: 333
            y: 231
            width: 135
            height: 61
            from: 10
            value:  grafcet.D7
            to: 5000
            stepSize: 5
            onValueChanged:
                grafcet.D7 = arret_etiqE.value
        }



        Text {
            id: text4
            x: 159
            y: 148
            text: qsTr("RETARD")
            font.pixelSize: 25
        }

        Text {
            id: text7
            x: 159
            y: 247
            text: qsTr("ARRET")
            font.pixelSize: 25
        }





        }

        Item {

            width: 800
            height: 480
            id: contreEtiquette
            clip: false
            visible: true


            Text {
                id: text42
                x: 217
                y: 41
                text: qsTr("CONTRE ETIQUETTE")
                font.underline: true
                font.bold: true
                font.pixelSize: 35
            }

            SpinBox {
                id: retard_etiq2
                x: 333
                y: 132
                width: 135
                height: 61
                from: 10
                value: grafcet.D5
                to: 5000
                stepSize: 5
                onValueChanged:
                    grafcet.D5 = retard_etiq2.value

            }

        SpinBox {
            id: arret_etiq_CE
            x: 333
            y: 231
            width: 135
            height: 61
            from: 10
            value: grafcet.D8
            to: 5000
            stepSize: 5
            onValueChanged:
                grafcet.D8 = arret_etiq_CE.value
        }


        Text {
            id: text5
            x: 158
            y: 148
            text: qsTr("RETARD")
            font.pixelSize: 25
        }

        Text {
            id: text8
            x: 158
            y: 247
            text: qsTr("ARRET")
            font.pixelSize: 25
        }




}
        Item{
            width: 800
            height: 480

            id: etiquetteContreEtiquette
            visible: true

            Text {
                id: text43
                x: 194
                y: 41
                text: "DOUBLE ETIQUETTES"
                font.underline: true
                font.bold: true
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.NoWrap
                font.pixelSize: 35
            }
            SpinBox {
                id: retard_etiq_ECE

                x: 333
                y: 132
                width: 135
                height: 61
                from: 10
                value: grafcet.D9
                to: 5000
                stepSize: 5
                onValueChanged:
                    grafcet.D9 = retard_etiq_ECE.value
            }

            SpinBox {
                id: arret_etiq_ECE
                x: 333
                y: 231
                width: 135
                height: 61
                from: 10
                value: grafcet.D10
                to: 5000
                stepSize: 5
                onValueChanged:
                    grafcet.D10 = arret_etiq_ECE.value
            }

            Text {
                id: text9
                x: 159
                y: 148
                text: qsTr("RETARD")
                font.pixelSize: 25
            }

            Text {
                id: text10
                x: 159
                y: 247
                text: qsTr("ARRET")
                font.pixelSize: 25
            }

        }
    }



        PageIndicator {
            y: 338
            visible: true
            anchors.horizontalCenterOffset: 0
            anchors.bottomMargin: 122
            count: svSettingsContainer.count
            currentIndex: svSettingsContainer.currentIndex

            anchors.bottom: svSettingsContainer.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }

    }





