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
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick 2.7
import Qt.labs.settings 1.0



Item {
    width: 800
    height: 480 
    Button {
        id: button
        x: 622
        y: 223
        width: 117
        height: 94
        text: qsTr("Button")
        onClicked: close()
    }

    SpinBox {
        id: secu_reperage
        x: 271
        y: 224
        width: 135
        height: 61
        from: 10
        value: grafcet.D3
        to: 5000
        stepSize: 50
        onValueChanged:
            grafcet.D3 = secu_reperage.value
    }

    SpinBox {
        id: retard_reperage
        x: 271
        y: 137
        width: 135
        height: 61
        from: 10
        value: grafcet.D2
        to: 5000
        stepSize: 10
        onValueChanged:
            grafcet.D2 = retard_reperage.value

    }

    SpinBox {
        id: secu_etiq
        x: 446
        y: 137
        width: 135
        height: 61
        from: 10
        value: grafcet.D6
        to: 5000
        stepSize: 50
        onValueChanged:
            grafcet.D6 = secu_etiq.value

    }

    SpinBox {
        id: retard_detect
        x: 271
        y: 50
        width: 135
        height: 61
        editable: false
        from: 10
        value: grafcet.D1
        stepSize: 10
        to: 5000
        onValueChanged:
            grafcet.D1 = retard_detect.value
    }

    SpinBox {
        id: lissage
        x: 446
        y: 50
        width: 135
        height: 61
        from: 10
        value: grafcet.D11
        to: 5000
        stepSize: 10
        onValueChanged:
            grafcet.D11 = lissage.value
    }

    Text {
        id: text1
        x: 10
        y: 65
        text: qsTr("RETARD DETEC")
        font.pixelSize: 25
    }

    Text {
        id: text11
        x: 606
        y: 59
        width: 114
        height: 36
        text: qsTr("LISSAGE")
        font.pixelSize: 25
    }


    Text {
        id: text2
        x: 10
        y: 152
        text: qsTr("RETARD REPERAGE")
        font.pixelSize: 25
    }

    Text {
        id: text3
        x: 8
        y: 239
        text: qsTr("SECU REPERAGE")
        font.pixelSize: 25
    }

    Text {
        id: text6
        x: 606
        y: 152
        text: qsTr("SECU ETIQ")
        font.pixelSize: 25
    }




    Label {
        id: co2
        x: 346
        y: 300
        text: grafcet.C2
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 20
    }
    Text {
        x: 291
        y: 306
        text: qsTr("CMP:")
        font.bold: true
        font.pixelSize: 17
    }

}





