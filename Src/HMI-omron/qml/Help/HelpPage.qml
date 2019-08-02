

import QtQuick 2.7
import QtQuick.Controls 2.0 as QQ2
//import "../Style"
import QtQuick.Controls 1.4

//import QtMultimedia 5.8


Item {

     property string  v_source: ""
     property string ve1: "file:/home/pi/Videos/1-Elements_Ninette.mp4"
     property string ve2: "file:/home/pi/Videos/2-Demarrage_de_la_machine.mp4"
     property string ve3: "file:/home/pi/Videos/3_Mise_en_place_bande_detiquettes.mp4"
     property string ve4: "file:/home/pi/Videos/4-Reglages_avant_etiquetage.mp4"
     property string ve5: "file:/home/pi/Videos/5-Tendre_la_bande.mp4"
    width: 800
//    Rectangle {
//              id: container
//              x: 202
//              y: 10
//              width:414
//              height: 40
//              gradient: Gradient {
//                  GradientStop {
//                      position: 0
//                      color: "#3560b8"
//                  }

//                  GradientStop {
//                      position: 1
//                      color: "#d1c3d2"
//                  }
//              }
//              border.width: 1
//              border.color: "#110e0e"


//              MouseArea {
//                  id: mouseArea
//                  anchors.fill: parent
//                  onClicked: {  v_source = ve1 ;v1.visible = true;
//                      video1.play(); v1.enabled= true ;  }
//              }

//              Text {
//                  id: label
//                  text: "1-Elements Ninette"
//                  font.family: "Verdana"
//                  font.pointSize: 16
//                  anchors.centerIn: parent
//              }

//          }

//      Rectangle {
//              id: container1
//              x: 202
//              y: 80
//              width: 414
//              height: 40
//              gradient: Gradient {
//                  GradientStop {
//                      position: 0
//                      color: "#3560b8"
//                  }

//                  GradientStop {
//                      position: 1
//                      color: "#d1c3d2"
//                  }
//              }
//              border.width: 1
//              border.color: "#110e0e"


//              MouseArea {
//                  id: mouseArea1
//                  anchors.fill: parent
//                  onClicked: { v_source = ve2 ; v1.visible = true;
//                      video1.play();  }
//              }

//              Text {
//                  id: label1
//                  text: "2-Démarrage de la machine"
//                  font.family: "Verdana"
//                  font.pointSize: 16
//                  anchors.centerIn: parent
//              }


//          }

//      Rectangle {
//              id: container2
//              x: 202
//              y: 150
//              width: 414
//              height: 40
//              gradient: Gradient {
//                  GradientStop {
//                      position: 0
//                      color: "#3560b8"
//                  }

//                  GradientStop {
//                      position: 1
//                      color: "#d1c3d2"
//                  }
//              }
//              border.width: 1
//              border.color: "#110e0e"


//              MouseArea {
//                  id: mouseArea2
//                  anchors.fill: parent
//                  onClicked: { v_source = ve3 ; v1.visible = true;
//                      video1.play();   }
//              }

//              Text {
//                  id: label2
//                  text: "3-Mise en place bande d'étiquettes"
//                  font.family: "Verdana"
//                  font.pointSize: 16
//                  anchors.centerIn: parent
//              }



//          }

//      Rectangle {
//              id: container3
//              x: 202
//              y: 220
//              width: 414
//              height: 40
//              border.width: 1
//              MouseArea {
//                  id: mouseArea3
//                  anchors.fill: parent
//                  onClicked: { v_source = ve4 ; v1.visible = true;
//                      video1.play(); }
//              }

//              Text {
//                  id: label3
//                  text: "4-Réglages avant étiquetage"
//                  font.family: "Verdana"
//                  font.pointSize: 16
//                  anchors.centerIn: parent
//              }
//              border.color: "#110e0e"
//              gradient: Gradient {
//                  GradientStop {
//                      position: 0
//                      color: "#3560b8"
//                  }

//                  GradientStop {
//                      position: 1
//                      color: "#d1c3d2"
//                  }
//              }
//          }

//      Rectangle {
//              id: container4
//              x: 202
//              y: 290
//              width: 414
//              height: 40
//              border.width: 1
//              MouseArea {
//                  id: mouseArea4
//                  anchors.fill: parent
//                  onClicked: { v_source = ve5 ; v1.visible = true;
//                      video1.play();}

//              }

//              Text {
//                  id: label4
//                  text: "5-Tendre la_bande"
//                  font.family: "Verdana"
//                  font.pointSize: 16
//                  anchors.centerIn: parent
//              }
//              border.color: "#110e0e"
//              gradient: Gradient {
//                  GradientStop {
//                      position: 0
//                      color: "#3560b8"
//                  }

//                  GradientStop {
//                      position: 1
//                      color: "#d1c3d2"
//                  }
//              }
//          }



//      // V1

//          Item {
//              id: v1
//              x: 0
//              y: 0
//              width: 800
//              height: 480
//              /* visible: true */
//              visible: false

//                                  Video {
//                                      id: video1
//                                      source: v_source
//                                      x:0
//                                      y:0
//                                      width: 800
//                                      height: 400

//                                      //source:"file:/home/pi/Videos/1-Elements_Ninette.mp4"


//                                  MouseArea {
//                                      x: 252
//                                      y: 0
//                                      width: 297
//                                      height: 400
//                                      onClicked: {
//                                          video1.playbackState == MediaPlayer.PlayingState ? video1.pause() : video1.play()
//                                      }
//                                      onDoubleClicked: {
//                                          video1.stop() ; v1.visible=false ;

//                                      }
//                                  }

//                                  MouseArea {
//                                      x: 564
//                                      y: 0
//                                      width: 236
//                                      height: 400
//                                      onClicked: {
//                                          video1.seek(video1.position + 500)
//                                      }
//                                      onDoubleClicked: {

//                                          onPressed: video1.seek(video1.position + 5000)

//                                      }

//                                  }

//                                  MouseArea {
//                                      x: 0
//                                      y: 0
//                                      width: 236
//                                      height: 400
//                                      onClicked: {
//                                          video1.seek(video1.position - 500)
//                                      }
//                                      onDoubleClicked: {

//                                          onPressed: video1.seek(video1.position - 5000)

//                                      }
//                                  }
//      }}


  }















