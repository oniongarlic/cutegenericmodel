import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import org.tal.model 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("CuteGenericModel example app")

    menuBar: MenuBar {
            Menu {
                title: "File"
                Action {
                    text: qsTr("&Quit")
                    onTriggered: Qt.quit()
                }
            }
            Menu {
                title: "Edit"
                Action {
                    text: "Clear 1"
                    onTriggered: diModel.clear();
                }
                Action {
                    text: "Clear 2"
                    onTriggered: diModelL.clear();
                }
            }
    }

    header: ToolBar {
        RowLayout {

            ToolButton {
                text: "Asc"
                onClicked: diModel.sortByProperty('id', DummyItemModel.SortAsc);
            }
            ToolButton {
                text: "Desc"
                onClicked: diModel.sortByProperty('id', DummyItemModel.SortDesc);
            }
            ToolButton {
                text: "Date"
                onClicked: diModel.sortByProperty('timestamp', DummyItemModel.SortDesc);
            }
            ToolButton {
                text: "Time"
                onClicked: diModelL.sortByProperty('time', DummyItemModel.SortAsc);
            }
            ToolButton {
                text: "Sort Name"
                onClicked: diModel.sortByProperty('name');
            }
            TextField {
                Layout.minimumWidth: 64
                Layout.maximumWidth: 128
                placeholderText: "Search"
                onAccepted: {
                    console.debug("Search: "+text)
                    diModel.search("name", text)
                    console.debug(diModel.count)                   
                }
            }
            TextField {
                Layout.maximumWidth: 64
                placeholderText: "ID #"
                onAccepted: {
                    var i=parseInt(text);
                    console.debug("Parsed ID: "+i)
                    var o=diModel.getId(i)
                    if (o)
                        console.debug(o["name"])
                }
            }
            TextField {
                Layout.maximumWidth: 64
                placeholderText: "Key"
                onAccepted: {
                    var o=diModel.getKey(text)
                    if (o)
                        console.debug(o["name"])
                    else
                        console.debug("Nothing found with key"+text)
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
            height: cl.height+4
            Row {
                id: cl                
                spacing: 8

                Text {
                    text: itemID
                }
                Text {
                    text: name
                }
                Text {
                    text: timestamp
                }
                Text {
                    text: datestamp
                }
                Text {
                    text: time
                }
                Text {
                    text: geo
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug(index)

                    var q=wrapper.ListView.view.model.get(index)
                    console.debug(q["id"])
                    console.debug(q["name"])
                    console.debug(q["category"])
                    console.debug(q["timestamp"])
                    console.debug(q["datestamp"])
                    console.debug(q["time"])

                    // setData()
                    time="13:00";                    

                    var di=wrapper.ListView.view.model.getItem(index)
                    console.debug(di.name)
                    console.debug(di.time)

                    // Direct modification, call refresh()
                    di.name="Clicked!"
                    wrapper.ListView.view.model.refresh(index)
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
