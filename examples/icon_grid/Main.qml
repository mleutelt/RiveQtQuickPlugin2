import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RiveQtQuick

ApplicationWindow {
    width: 1480
    height: 1280
    visible: true
    title: "Interactive Icon Grid"

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#eef4f8" }
            GradientStop { position: 0.55; color: "#dce7f0" }
            GradientStop { position: 1.0; color: "#c5d6e6" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Pane {
            Layout.fillWidth: true
            padding: 16

            background: Rectangle {
                radius: 24
                color: "#102033"
                border.color: "#29435e"
            }

            RowLayout {
                anchors.fill: parent
                spacing: 18

                ColumnLayout {
                    spacing: 4

                    Label {
                        text: "Interactive Icon Field"
                        color: "#f7fbff"
                        font.pixelSize: 24
                        font.bold: true
                    }

                    Label {
                        text: "A simple 100x100 table of 100x100 buttons. Each button now runs one artboard from the icon set."
                        color: "#b9c9d8"
                        font.pixelSize: 13
                    }
                }

                Item { Layout.fillWidth: true }

                ColumnLayout {
                    spacing: 4

                    Label {
                        text: creditTitle + " by " + creditAuthor
                        color: "#f3f8fc"
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: creditLicense + "  |  " + creditStatus + "  |  " +
                              (iconGridModel.ready ? iconGridModel.artboardCount + " artboards loaded" : "reading artboards...")
                        color: "#aac0d3"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Button {
                    text: "Open credit"
                    onClicked: Qt.openUrlExternally(creditUrl)
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 32
            color: "#081320"
            border.color: "#23405b"
            border.width: 2
            clip: true

            TableView {
                id: iconTable
                anchors.fill: parent
                anchors.margins: 18
                clip: true
                reuseItems: true
                boundsBehavior: Flickable.StopAtBounds
                model: iconGridModel

                rowHeightProvider: function() {
                    return 100
                }

                columnWidthProvider: function() {
                    return 100
                }

                delegate: Button {
                    id: control
                    required property int artboardNumber
                    required property int row
                    required property int column

                    width: 100
                    height: 100
                    enabled: artboardNumber > 0
                    hoverEnabled: true

                    background: Rectangle {
                        radius: 12
                        color: control.down
                               ? "#ffffff"
                               : control.hovered
                                 ? Qt.lighter(cellAccent, 1.14)
                                 : cellAccent
                        border.color: "#23384c"
                        border.width: 1
                    }

                    readonly property color cellAccent: Qt.hsva(
                        (((row * 37) + (column * 19)) % 360) / 360.0,
                        0.18,
                        0.94,
                        1.0)

                    contentItem: Item {
                        RiveItem {
                            id: riveView
                            anchors.fill: parent
                            anchors.margins: 10
                            source: control.enabled ? exampleRiveUrl : ""
                            artboardIndex: artboardNumber
                            fit: RiveItem.Contain
                            interactive: true
                            enabled: false
                            hovered: control.hovered
                            playing: true
                        }
                    }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 28
                width: 360
                height: 136
                radius: 24
                color: "#0c1724"
                border.color: "#2d4b66"
                visible: !iconGridModel.ready

                Column {
                    anchors.centerIn: parent
                    spacing: 14

                    BusyIndicator {
                        anchors.horizontalCenter: parent.horizontalCenter
                        running: iconGridModel.errorString.length === 0
                    }

                    Label {
                        text: iconGridModel.errorString.length > 0
                              ? iconGridModel.errorString
                              : "Loading the icon artboards..."
                        color: "#eff6fb"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        width: 320
                        text: "Credit: " + creditTitle + " by " + creditAuthor + " (" + creditLicense + ")"
                        color: "#9fb4c8"
                        wrapMode: Text.Wrap
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
