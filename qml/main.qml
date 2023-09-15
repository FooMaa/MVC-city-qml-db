import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.11
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2

import "connector_qml.js" as ConnectorQML

import modelTable 2.1

Window {
    id: lol
    width: 750
    height: 450
    visible: true
    title: qsTr("City")

    property var dragIndex
    property var curIdx: view_tree.rootIndex
    property var idxForList


    Dialog {
        id: dialog_window
        title: "Changing name"
        height: 100
        width: 400
        x: lol.x + 100
        y: lol.y + 100
        modality : Qt.ApplicationModal


        contentItem: ColumnLayout {
            anchors.fill: parent

            Text {
                     id: label_text
                     text: "Change name object after drop"
                     verticalAlignment: Text.AlignBottom
                     horizontalAlignment: Text.AlignHCenter
                     Layout.fillWidth: true
                     padding: 0
            }

            TextField {
                id: text_name
                     Layout.fillWidth: true
                     placeholderText: "Take name"
            }

            Button {
                     text: "Changing"
                     Layout.fillWidth: true
                     onClicked: {

                         if( model_tree.changeObjectName( text_name.text ) ) {
                              dialog_window.close()
                          }

                         if ( typeof idxForList !== 'undefined' )
                         if ( idxForList !== 0 )
                         if ( typeof idxForList !== view_tree.rootIndex ) {
                             model_list.slot_insertToLineFromTree( idxForList )
                         }

                     }
            }

         }

    }

    DataModelTable {
        id: model_table
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {

            TreeView {
                id: view_tree
                Layout.fillHeight: true
                model: model_tree

                onDoubleClicked: {
                    model_list.slot_insertToLineFromTree( view_tree.currentIndex )
                    model_tree.setCurrentIndex( view_tree.currentIndex )
                }

                rowDelegate: Rectangle {
                        height: 20
                }

                itemDelegate: Item {
                    id: treeItem

                    DropArea {
                       id: dropArea
                       anchors.fill: parent

                       onDropped: {
                           if ( dragIndex !== styleData.index && dragIndex !== view_tree.rootIndex && styleData.index !== view_tree.rootIndex ) {
                               curIdx = view_tree.rootIndex

                               if ( idxForList !== 0 )
                               if ( typeof idxForList !== 'undefined' )
                               if ( model_tree.checkIndex( idxForList, dragIndex ) ) {
                                   idxForList = 0
                               }

                               if ( model_tree.changeParent( dragIndex, styleData.index ) ) {
                                  dialog_window.open()
                               }

                               if ( idxForList !== 0 )
                               if ( typeof idxForList !== 'undefined' )
                               if( typeof idxForList !== view_tree.rootIndex ) {
                                   model_list.slot_insertToLineFromTree( idxForList )
                               }

                           }

                       }

                     }

                     Rectangle {
                         id: dragRect
                         width: parent.width
                         height: 20
                         anchors.horizontalCenter: parent.horizontalCenter
                         anchors.verticalCenter: parent.verticalCenter
                         Component.onCompleted: {
                             view_tree.expand( styleData.index );
                         }

                         color: {

                           if ( rowMouseArea.containsMouse ) {
                                  return ConnectorQML.COLOR_GRAY_OPAQUE
                              }
                           else if ( typeof styleData.index !== 'undefined' && typeof curIdx !== 'undefined' ) {
                               if ( curIdx === styleData.index ) {
                                    return ConnectorQML.COLOR_LIGHTBLUE
                               }
                               else if ( dropArea.exited ) {
                                   return ConnectorQML.COLOR_TRANSPARENT
                               }
                           }
                           else if ( dropArea.exited ) {
                               return ConnectorQML.COLOR_TRANSPARENT
                           }
                           else {
                                  return ConnectorQML.COLOR_GRAY_TRANSPARENT
                              }
                         }


                         Text {
                             id: nameText
                             x: 15
                             anchors.verticalCenter: parent.verticalCenter
                             color: ConnectorQML.COLOR_BLACK
                             text: styleData.value
                         }

                       MouseArea {
                             id: rowMouseArea
                             anchors.fill: parent
                             hoverEnabled: true
                             property bool hovered: false
                             acceptedButtons: Qt.LeftButton | Qt.RightButton
                             drag.target: dragRect
                             drag.onActiveChanged: {

                               if ( rowMouseArea.drag.active ) {
                                   dragIndex = styleData.index
                               }

                               dragRect.Drag.drop();

                             }

                             onDoubleClicked: {
                               model_list.slot_insertToLineFromTree( styleData.index )
                               model_tree.setCurrentIndex( styleData.index )
                               idxForList = styleData.index
                             }

                             onClicked: {
                               curIdx = styleData.index
                             }

                         }

                         states: [ State {
                               when: dragRect.Drag.active

                               ParentChange {
                                   target: dragRect
                                   parent: view_tree; width: dragRect.width; height: dragRect.height
                               }

                               AnchorChanges {
                                   target: dragRect
                                   anchors.horizontalCenter: undefined
                                   anchors.verticalCenter: undefined
                               }
                            }
                         ]

                         Drag.active: rowMouseArea.drag.active
                         Drag.supportedActions:  Qt.CopyAction;
                         Drag.hotSpot.x: dragRect.width / 2
                         Drag.hotSpot.y: dragRect.height / 2
                     }
                 }

                 TableViewColumn {
                     id: table
                     role: "name"
                     title: "TreeView"
                 }

              }

                 ColumnLayout {
                   Layout.fillHeight: true
                   Layout.fillWidth: true

                 Button {
                    id: btn_sort
                    Layout.fillHeight: false
                    text: qsTr("Sort")
                    Layout.fillWidth: true
                    checkable: true
                    onClicked:  model_proxy_sort.sortingListView( btn_sort.checked )
                 }

                 TextField {
                     id: searching
                     Layout.fillWidth: true
                     placeholderText: "Search in list view"
                     onTextChanged: model_proxy_sort.filteringListView( text )
                 }

                 ListView {
                     id: view_list
                     Layout.fillHeight: true
                     Layout.fillWidth: true
                     snapMode: ListView.SnapToItem
                     model: model_proxy_sort
                     spacing: 3

                     ScrollBar.vertical: ScrollBar {
                           id: verticalScrollBar
                           active: true
                           orientation: Qt.Vertical
                           opacity: active ? 1:0
                           Behavior on opacity { NumberAnimation { duration: 500 } }

                           contentItem: Rectangle {
                               implicitWidth: 7
                               radius: 2
                               border.width: 1

                               implicitHeight: parent.height
                               color: ConnectorQML.COLOR_GRAY_TRANSPARENT
                           }
                     }

                     delegate: Rectangle {
                         id: rect_list
                         width: parent.width
                         anchors.horizontalCenter: parent.horizontalCenter
                         height: 25
                         border.color: ConnectorQML.COLOR_BLACK
                         border.width: 0.5

                         Text {
                            text: name
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                         }

                          MouseArea {
                             id: mouse_area_list
                             anchors.fill: parent
                             hoverEnabled: true
                             onDoubleClicked: {
                                   model_list.slot_insertToLineFromTree( model_tree.seeObjectByName( name ) )
                               }
                           }

                           color: {

                               if ( mouse_area_list.containsMouse ) {
                                   return ConnectorQML.COLOR_GRAY_OPAQUE
                               }
                               else {
                                   return ConnectorQML.COLOR_GRAY_TRANSPARENT
                               }

                           }

                       }
                   }

               }

        }

        Shortcut {
            id: shortcut
            sequence: "Ctrl+0"
            onActivated: model_table.slot_realiseTable( model_proxy_sort, model_list, model_tree )
        }

        TableView {
            id: view_table
            Layout.fillWidth: true
            transformOrigin: Item.Left
            clip: true
            model: model_table

            TableViewColumn {
               role: "name"
               title: "Objects"
               resizable: false
               width: 250
            }

            TableViewColumn{
               role: "address"
               title: "Address"
               resizable: false
               width: 250
            }

            TableViewColumn {
               role: "property"
               title: "Property"
               resizable: false
               width: 250
            }

        }

        Button {
            id: btn_add
            text: qsTr("Add")
            Layout.fillWidth: true
            onClicked: {
                model_tree.slot_insertValueFromLineEdit( curIdx )

                if ( model_tree.checkAdd( curIdx ) ) {
                    dialog_window.open()
                }

                if( curIdx !== view_tree.rootIndex && curIdx === idxForList )   {
                    model_list.slot_insertToLineFromTree( curIdx )
                }
            }
        }
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
