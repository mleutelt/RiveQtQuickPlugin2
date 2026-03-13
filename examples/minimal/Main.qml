import RiveQtQuick
import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    color: "#20252b"
    title: "Rive Qt Quick Minimal"

    RiveItem {
        objectName: "riveView"
        anchors.fill: parent
        anchors.margins: 32
        source: exampleRiveUrl
    }
}
