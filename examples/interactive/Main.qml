import RiveQtQuick
import QtQuick.Layouts
import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 800
    height: 600
    visible: true
    color: "#101418"
    title: "Rive Qt Quick Interactive"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        RiveItem {
            id: riveView
            objectName: "riveView"
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: exampleRiveUrl
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: riveView.playing ? "Pause" : "Play"
                onClicked: riveView.playing ? riveView.pause() : riveView.play()
            }

            Button {
                text: "Trigger"
                onClicked: riveView.fireTrigger("Trigger 1")
            }

            Slider {
                Layout.fillWidth: true
                from: 0
                to: 100
                value: 50
                onValueChanged: riveView.setNumber("Number 1", value)
            }
        }
    }
}
