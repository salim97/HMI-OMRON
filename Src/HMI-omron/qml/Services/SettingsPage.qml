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
import Qt.labs.settings 1.0
import "../Style"

Item {

    Settings {
        id: settings
        property alias etiquette: etiquetteSwitch.checked
        property alias doubleEtiquette: doubleEtiqSwitch.checked
        property alias reperageCran: reperageCranSwitch.checked
        property alias reperageEtiq: reperageEtiquetteSwitch.checked


       // property alias contrast: contrastSlider.value
        //property alias brightness: brightnessSlider.value
    }

    QQC2.SwipeView {
        id: svSettingsContainer

        anchors.fill: parent

        Item {
            id: settingsPage1

            Column {
                anchors.centerIn: parent
                spacing: 25

                Row {
                    spacing: 185
                    Text {
                        id: text1
                        text: qsTr("ETIQUETTE")
                        font.pixelSize: 30
                    }
                    QQC2.Switch {
                        id: etiquetteSwitch
                        x: 351
                        anchors.verticalCenter: parent.verticalCenter
                        checked: settings.etiquette
                    }

                }

                Row {
                    spacing: 50
                    Text {
                        id: text2
                        text: qsTr("DOUBLE ETIQUETTE")
                        font.pixelSize: 30
                    }

                    QQC2.Switch {
                        id: doubleEtiqSwitch
                        anchors.verticalCenter: parent.verticalCenter
                        checked: settings.doubleEtiquette
                    }

                }

            }
        }

        Item {
            id: settingsPage2

            Column {
                anchors.centerIn: parent
                spacing: 25

                Row {
                    spacing: 130
                    Text {
                        id: text3
                        text: qsTr("REPERAGE CRAN")
                        font.pixelSize: 30
                    }
                    QQC2.Switch {
                        id: reperageCranSwitch
                        x: 351
                        anchors.verticalCenter: parent.verticalCenter
                        checked: settings.reperageCran
                    }

                }

                Row {
                    spacing: 50
                    Text {
                        id: text4
                        text: qsTr("REPERAGE ETIQUETTE")
                        font.pixelSize: 30
                    }

                    QQC2.Switch {
                        id: reperageEtiquetteSwitch
                        anchors.verticalCenter: parent.verticalCenter
                        checked: settings.reperageEtiq
                    }

                }



           }
        }
   }

    QQC2.PageIndicator {
        y: 412
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 48
        count: svSettingsContainer.count
        currentIndex: svSettingsContainer.currentIndex

        anchors.bottom: svSettingsContainer.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
