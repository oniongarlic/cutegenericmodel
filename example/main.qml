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
                text: "Clear 1"
                onClicked: diModel.clear();
            }
            ToolButton {
                text: "Clear 2"
                onClicked: diModelL.clear();
            }
            ToolButton {
                text: "Asc"
                onClicked: diModel.sortByProperty('id', DummyItemModel.SortAsc);
            }
            ToolButton {
                text: "Desc"
                onClicked: diModel.sortByProperty('id', DummyItemModel.SortDesc);
            }
            ToolButton {
                text: "Sort Name"
                onClicked: diModel.sortByProperty('name');
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

    RowLayout {
        anchors.fill: parent

        ListView {
            id: lv1
            model: diModel
            delegate: dummyItemDelegate
            clip: true
            spacing: 8
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }

        ListView {
            id: lv2
            model: diModelL
            delegate: dummyItemDelegate
            clip: true
            spacing: 8
            Layout.fillWidth: true;
            Layout.fillHeight: true;
        }

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

                    var q=wrapper.ListView.view.model.get(index)
                    console.debug(q["name"])
                    console.debug(q["category"])

                    var di=wrapper.ListView.view.model.getItem(index)
                    console.debug(di.name)

                }
                onPressAndHold: {
                    wrapper.ListView.view.model.remove(index)
                }
            }
        }
    }

    footer: ToolBar {
        RowLayout {
            Text {
                text: diModel.count
            }
            Text {
                text: diModelL.count
            }
        }
    }

}
