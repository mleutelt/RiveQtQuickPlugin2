import QtQuick
import QtQuick.Controls
import QtQuick.Window
import RiveQtQuick

Window {
    visible: false
    width: 1
    height: 1

    RiveItem {
        anchors.fill: parent
    }

    Button {
        visible: false
    }

    Switch {
        visible: false
    }

    Slider {
        visible: false
    }
}
