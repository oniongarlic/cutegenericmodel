import QtQuick 2.9

Rectangle {
    id: w
    property bool editing: false
    width: editing ? t.implicitWidth*2 : t.implicitWidth
    height: t.implicitHeight

    property string text: ""

    signal accecpted();

    border.color: editing ? "black" : "transparent"
    border.width: editing ? 1 : 0

    Text {
        id: t
        anchors.fill: parent
        visible: !w.editing
        text: w.text
    }
    TextInput {
        id: ti
        anchors.fill: parent
        visible: w.editing
        width: visible ? t.implicitWidth*2 : t.implicitWidth
        z: 100
        selectByMouse: true
        onVisibleChanged: if (visible) {
                              text=w.text
                              forceActiveFocus();
                          }
        onFocusChanged: if (!focus && visible) w.editing=false;
        onAccepted: {
            w.text=text
            w.accecpted();
            w.editing=false
            text=""
        }
    }
}
