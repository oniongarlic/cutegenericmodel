import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import org.tal.model 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("CuteGenericModel example app")

    header: ToolBar {
        RowLayout {
            ToolButton {
                text: "Quit"
                onClicked: Qt.quit()
            }
            ToolButton {
                text: "Clear all"
                onClicked: diModel.clear();
            }
            TextField {
                placeholderText: "Search"
                onAccepted: {
                    console.debug("Search: "+text)
                    diModel.search(text)
                }
            }
        }
    }

    ListView {
        id: lv
        model: diModel
        anchors.fill: parent
        delegate: dummyItemDelegate
        clip: true
        spacing: 8
    }

    Component {
        id: dummyItemDelegate
        Rectangle {
            id: wrapper
            color: "#9acc9a"
            radius: 4
            width: parent.width
            height: cl.height
            Row {
                id: cl
                anchors.margins: 4
                spacing: 8
                Text {
                    text: itemID
                }
                Text {
                    text: name
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug(index)

                    var q=diModel.get(index)
                    console.debug(q["name"])
                    console.debug(q["category"])

                    var di=diModel.getItem(index)
                    console.debug(di.name)

                }
                onPressAndHold: {
                    diModel.remove(index)
                }
            }
        }
    }

    footer: ToolBar {
        RowLayout {
            Text {
                text: diModel.count
            }
        }
    }

}
