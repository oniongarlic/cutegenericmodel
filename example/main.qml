import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
                text: qsTr("&Json")
                onTriggered: {
                    console.debug(diModel.toJson());
                }
            }
            Action {
                text: "From JSON"
                onTriggered: {
                    diModel.fromJson('
                                [
                                    {
                                        "category": "JSON",
                                        "enabled": false,
                                        "key": "json-test",
                                        "name": "JSON Import",
                                        "time": "12:34:56",
                                        "timestamp": "2024-11-11T22:50:10"
                                    }
                                ]'
                                );
                }
            }

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
                text: "ID Asc"
                onClicked: diModel.sortByProperty('id', DummyItemModel.SortAsc);
            }
            ToolButton {
                text: "ID Desc"
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
                selectByMouse: true
                onAccepted: {
                    console.debug("Search: "+text)
                    if (text!='')
                        diModel.search("name", text)
                    else
                        diModel.clearSearch();
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
                placeholderText: "Key1"
                onAccepted: {
                    var o=diModel.getKey(text)
                    if (o)
                        console.debug(o["name"])
                    else
                        console.debug("Nothing found with key"+text)
                }
            }
            TextField {
                Layout.maximumWidth: 64
                placeholderText: "Key2"
                onAccepted: {
                    var o=diModelL.getKey(text)
                    if (o)
                        console.debug(o["name"])
                    else
                        console.debug("Nothing found with key"+text)
                }
            }
            ComboBox {
                id: comboTest
                textRole: "name"
                valueRole: "key"
                model: diModelL
                Layout.fillWidth: true
                onActivated: {
                    console.debug("******************************")
                    var tmp=model.get(currentIndex);
                    console.debug("Obj", currentIndex, tmp.id, tmp.key)
                    console.debug("Value", currentValue)
                    console.debug("******************************")
                }
            }
        }
    }

    Component {
        id: headerComponent
        RowLayout {
            id: headerRow
            spacing: 1
            z: 2
            width: parent.width
            function itemAt(index) { return r2.itemAt(index) }
            Repeater {
                id: r2
                model: ["ID", "Item name & title", "TimeStamp", "DateStamp", "Time", "Geo"]
                Label {
                    text: modelData
                    font.bold: true
                    font.pixelSize: 14
                    padding: 4
                    background: Rectangle { color: "grey" }
                    Layout.fillWidth: true
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.debug(index)
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8

        ListView {
            id: lv1
            model: diModel
            delegate: dummyItemDelegate
            clip: true
            spacing: 8
            highlight: highlightBar
            highlightFollowsCurrentItem: true
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            header: headerComponent

            ScrollBar.vertical: ScrollBar { }

            remove: Transition {
                NumberAnimation { property: "opacity"; to: 0; duration: 200 }
            }
        }

        ListView {
            id: lv2
            model: diModelL
            delegate: dummyItemDelegate
            clip: true
            spacing: 8
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            highlight: highlightBar
            highlightFollowsCurrentItem: true
            headerPositioning: ListView.OverlayHeader
            header: headerComponent

            ScrollBar.vertical: ScrollBar { }
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            color: "green"
        }
    }

    Component {
        id: dummyItemDelegate
        Rectangle {
            id: wrapper
            color: "transparent"
            radius: 4
            width: ListView.view.width
            height: cl.height+8
            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    console.debug("DoubleClicked!")
                    nameEditor.editing=!nameEditor.editing
                }
            }
            Row {
                id: cl
                spacing: 4
                Text {
                    text: itemID
                    width: wrapper.ListView.view.headerItem.itemAt(0).width
                }
                EditableText {
                    id: nameEditor
                    text: name
                    width: wrapper.ListView.view.headerItem.itemAt(1).width
                    onAccecpted: {
                        console.debug(index)
                        console.debug(text)
                        name=text;
                        //var di=wrapper.ListView.view.model.getItem(index)
                        //di.name=text;
                        //wrapper.ListView.view.model.refresh(index)
                    }
                }
                Text {
                    text: timestamp
                    width: wrapper.ListView.view.headerItem.itemAt(2).width
                    clip: true
                    elide: Text.ElideMiddle
                }
                Text {
                    text: datestamp
                    width: wrapper.ListView.view.headerItem.itemAt(3).width
                    clip: true
                    elide: Text.ElideRight
                }
                Text {
                    text: time
                    width: wrapper.ListView.view.headerItem.itemAt(4).width
                }
                Text {
                    text: geo
                    width: wrapper.ListView.view.headerItem.itemAt(5).width
                }
            }
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    console.debug(index)

                    wrapper.ListView.view.currentIndex=index;

                    // get() we are given a QVariantMap
                    var q=wrapper.ListView.view.model.get(index)
                    console.debug(q["id"] + q["name"])
                    console.debug(q["category"] + q["timestamp"]+q["datestamp"])

                    // Set model property trough role name, setData() in C++ model
                    time=new Date();
                    //name="CLICKED!"

                    // Get the QObject derived object, in this example case a DummyItem
                    var di=wrapper.ListView.view.model.getItem(index)
                    console.debug(di.name)
                    console.debug(di.time)

                    // Direct modification, call refresh()
                    //di.name="Clicked!"
                    //wrapper.ListView.view.model.refresh(index)
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
