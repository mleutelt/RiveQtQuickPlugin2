import RiveQtQuick
import QtQuick.Window
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    width: 1500
    height: 920
    visible: true
    color: "#11161d"
    title: "QtQuickRivePlugin2 demo"
    property url activePreviewSource: ""
    property url pendingPreviewSource: ""
    property bool previewSwitchPendingClear: false
    property var currentMeta: catalog.currentMetadata
    property bool metaCanDownload: Boolean(currentMeta.canDownload)
    property bool metaIsDownloading: Boolean(currentMeta.isDownloading)
    property bool metaIsForHire: Boolean(currentMeta.isForHire)
    property bool livePreviewVisible: activePreviewSource.toString().length > 0 &&
                                      activePreviewSource.toString() === catalog.currentRiveSource.toString()
    property bool centralActionEnabled: catalog.currentHasPreview || metaCanDownload
    property string centralActionText: metaIsDownloading
                                       ? "Downloading..."
                                       : catalog.currentHasPreview
                                         ? "Load live preview"
                                         : metaCanDownload
                                           ? "Download local copy"
                                           : "Poster only"

    function schedulePreviewSourceRefresh(forceReload) {
        pendingPreviewSource = catalog.currentHasPreview ? catalog.currentRiveSource : ""

        if (!forceReload && activePreviewSource.toString() === pendingPreviewSource.toString()) {
            return
        }

        if (activePreviewSource.toString().length > 0) {
            previewSwitchPendingClear = true
            activePreviewSource = ""
            return
        }

        previewSwitchPendingClear = false
        sourceSwitchTimer.restart()
    }

    Timer {
        id: sourceSwitchTimer
        interval: 0
        repeat: false
        onTriggered: activePreviewSource = pendingPreviewSource
    }

    component SectionTitle: Label {
        color: "#e8edf2"
        font.pixelSize: 15
        font.bold: true
    }

    component MetaRow: RowLayout {
        required property string label
        required property string value
        Layout.fillWidth: true
        spacing: 8

        Label {
            text: parent.label
            color: "#8fa1b3"
            font.pixelSize: 12
            Layout.preferredWidth: 90
        }

        Label {
            text: parent.value
            color: "#f3f6f9"
            font.pixelSize: 12
            wrapMode: Text.Wrap
            Layout.fillWidth: true
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Frame {
            Layout.preferredWidth: 360
            Layout.minimumWidth: 360
            Layout.maximumWidth: 360
            Layout.fillHeight: true
            padding: 0
            background: Rectangle {
                radius: 18
                color: "#16202a"
                border.color: "#24313f"
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 14

                SectionTitle { text: "Marketplace" }

                TextField {
                    Layout.fillWidth: true
                    placeholderText: "Try a title, author or tag"
                    text: catalog.searchText
                    onTextChanged: catalog.searchText = text
                }

                Label {
                    Layout.fillWidth: true
                    color: "#8fa1b3"
                    wrapMode: Text.Wrap
                    font.pixelSize: 12
                    text: "This list comes straight from the public featured feed. Most entries start in poster mode and turn into a live preview once you download the runtime file."
                }

                ListView {
                    id: catalogList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: catalog
                    spacing: 10
                    clip: true
                    footer: Item {
                        width: ListView.view.width
                        height: catalog.isLoadingMore || catalog.hasMoreRemoteEntries ? 56 : 0

                        Label {
                            anchors.centerIn: parent
                            visible: catalog.isLoadingMore || catalog.hasMoreRemoteEntries
                            color: "#8fa1b3"
                            font.pixelSize: 12
                            text: catalog.isLoadingMore ? "Pulling in a few more..." : "Keep scrolling for more"
                        }
                    }
                    onContentYChanged: {
                        if (catalog.isLoadingMore || !catalog.hasMoreRemoteEntries)
                            return
                        if (contentY + height >= contentHeight - 320)
                            catalog.loadMore()
                    }
                    onCountChanged: {
                        if (catalog.isLoadingMore || !catalog.hasMoreRemoteEntries)
                            return
                        if (contentHeight <= height + 200)
                            catalog.loadMore()
                    }

                    delegate: Rectangle {
                        required property int index
                        required property string title
                        required property string author
                        required property string license
                        required property var tags
                        required property string description
                        required property string previewImageUrl
                        required property int reactionCount
                        required property bool current
                        required property bool isForHire
                        required property string availability
                        required property bool hasPreview

                        width: ListView.view.width
                        height: 138
                        radius: 16
                        color: current ? "#213241" : "#19242e"
                        border.color: current ? "#63c7ff" : "#2a3947"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: catalog.currentIndex = index
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 14
                            spacing: 12

                            Rectangle {
                                Layout.preferredWidth: 94
                                Layout.fillHeight: true
                                radius: 12
                                color: "#101820"
                                border.color: "#314150"
                                clip: true

                                Image {
                                    anchors.fill: parent
                                    source: previewImageUrl
                                    fillMode: Image.PreserveAspectCrop
                                    asynchronous: true
                                    cache: true
                                    smooth: true
                                    visible: status === Image.Ready
                                }

                                Rectangle {
                                    anchors.fill: parent
                                    visible: parent.children[0].status !== Image.Ready
                                    color: "#17212a"

                                    Label {
                                        anchors.centerIn: parent
                                        text: title.length > 0 ? title.charAt(0).toUpperCase() : "R"
                                        color: "#a8c2d8"
                                        font.pixelSize: 30
                                        font.bold: true
                                    }
                                }
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 8

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: title
                                        color: "#f5f8fb"
                                        font.pixelSize: 15
                                        font.bold: true
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }

                                    Rectangle {
                                        visible: isForHire
                                        radius: 999
                                        color: "#21533c"
                                        implicitWidth: hireLabel.implicitWidth + 14
                                        implicitHeight: hireLabel.implicitHeight + 6

                                        Label {
                                            id: hireLabel
                                            anchors.centerIn: parent
                                            text: "For hire"
                                            color: "#baf0ce"
                                            font.pixelSize: 11
                                        }
                                    }
                                }

                                Label {
                                    text: "by " + author
                                    color: "#91a3b5"
                                    font.pixelSize: 12
                                }

                                Label {
                                    Layout.fillWidth: true
                                    text: description
                                    color: "#d4dee7"
                                    font.pixelSize: 12
                                    wrapMode: Text.Wrap
                                    maximumLineCount: 3
                                    elide: Text.ElideRight
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 8

                                    Repeater {
                                        model: tags
                                        delegate: Rectangle {
                                            required property var modelData
                                            radius: 999
                                            color: "#23303d"
                                            implicitWidth: tagLabel.implicitWidth + 12
                                            implicitHeight: tagLabel.implicitHeight + 4

                                            Label {
                                                id: tagLabel
                                                anchors.centerIn: parent
                                                text: modelData
                                                color: "#b9c7d4"
                                                font.pixelSize: 10
                                            }
                                        }
                                    }

                                    Item { Layout.fillWidth: true }

                                    Label {
                                        text: reactionCount > 0 ? reactionCount + " likes" : "fresh"
                                        color: "#8fa1b3"
                                        font.pixelSize: 11
                                    }

                                    Label {
                                        text: hasPreview ? availability : "poster only"
                                        color: hasPreview ? "#8ee7b0" : "#d7b87a"
                                        font.pixelSize: 11
                                    }

                                    Label {
                                        text: license
                                        color: "#8fa1b3"
                                        font.pixelSize: 11
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 520
            padding: 0
            background: Rectangle {
                radius: 18
                color: "#171d24"
                border.color: "#24313f"
            }

            Item {
                anchors.fill: parent

                RiveItem {
                    id: riveView
                    objectName: "riveView"
                    anchors.fill: parent
                    anchors.margins: 18
                    source: activePreviewSource
                    fit: RiveItem.Contain
                    Component.onCompleted: schedulePreviewSourceRefresh(false)
                }

                Connections {
                    target: catalog
                    function onCurrentIndexChanged() {
                        schedulePreviewSourceRefresh(false)
                    }
                    function onCurrentRiveSourceChanged() {
                        schedulePreviewSourceRefresh(false)
                    }
                    function onCurrentHasPreviewChanged() {
                        schedulePreviewSourceRefresh(false)
                    }
                }

                Connections {
                    target: riveView
                    function onStatusChanged() {
                        if (!previewSwitchPendingClear)
                            return
                        if (riveView.status !== RiveItem.Null)
                            return
                        previewSwitchPendingClear = false
                        sourceSwitchTimer.restart()
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 18
                    radius: 14
                    visible: !livePreviewVisible
                    color: "#11161d"
                    border.color: "#2b3948"
                    clip: true

                    Image {
                        id: heroPoster
                        anchors.fill: parent
                        source: String(currentMeta.previewImageUrl || "")
                        fillMode: Image.PreserveAspectCrop
                        asynchronous: true
                        cache: true
                        smooth: true
                        visible: status === Image.Ready
                    }

                    Rectangle {
                        anchors.fill: parent
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#110d1116" }
                            GradientStop { position: 0.52; color: "#6611161d" }
                            GradientStop { position: 1.0; color: "#e611161d" }
                        }
                    }

                    Column {
                        anchors.centerIn: parent
                        width: Math.min(parent.width - 80, 360)
                        spacing: 16
                        visible: centralActionEnabled || metaIsDownloading

                        Button {
                            id: centerActionButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width
                            height: 68
                            enabled: centralActionEnabled
                            text: centralActionText

                            background: Rectangle {
                                radius: 20
                                color: centerActionButton.enabled ? "#ff8d71" : "#45515d"
                                border.color: centerActionButton.enabled ? "#ffd0c1" : "#697684"
                                border.width: 2
                            }

                            contentItem: Label {
                                text: centerActionButton.text
                                color: "#fff8f4"
                                font.pixelSize: 22
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            onClicked: {
                                if (catalog.currentHasPreview)
                                    schedulePreviewSourceRefresh(true)
                                else
                                    catalog.downloadCurrent()
                            }
                        }

                        Label {
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.Wrap
                            color: "#d5dee7"
                            font.pixelSize: 13
                            text: catalog.currentHasPreview
                                  ? "Open the real file and start poking at the artboards, state machines and inputs."
                                  : metaCanDownload
                                    ? "Grab the actual .riv file locally. The poster will switch to a live preview right after."
                                    : metaIsDownloading
                                      ? "The runtime file is on its way."
                                      : ""
                        }
                    }

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: 26
                        spacing: 12

                        Label {
                            text: heroPoster.status === Image.Ready ? "Poster view" : "Nothing to preview yet"
                            color: "#eef4fa"
                            font.pixelSize: 24
                            font.bold: true
                        }

                        Label {
                            width: parent.width
                            text: catalog.currentHasPreview
                                  ? "This one has a local runtime file, so you can actually poke at it."
                                  : metaIsDownloading
                                    ? "Downloading the runtime file now. As soon as it lands in the cache, the live preview swaps in."
                                    : metaCanDownload
                                      ? "Right now this is just the marketplace poster. The big button in the middle grabs the live file."
                                      : "This one is poster and metadata only for now."
                            color: "#d0dbe5"
                            font.pixelSize: 13
                            wrapMode: Text.Wrap
                        }

                        Label {
                            visible: String(currentMeta.downloadError || "").length > 0
                            width: parent.width
                            text: String(currentMeta.downloadError || "")
                            color: "#ff9e9e"
                            font.pixelSize: 12
                            wrapMode: Text.Wrap
                        }

                        Label {
                            visible: metaIsDownloading &&
                                     Number(currentMeta.downloadTotalBytes || 0) > 0
                            text: "Download progress: " +
                                  Math.round(Number(currentMeta.downloadBytes || 0) * 100 /
                                             Number(currentMeta.downloadTotalBytes || 0)) + "%"
                            color: "#8ee7b0"
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }

        Frame {
            Layout.preferredWidth: 400
            Layout.minimumWidth: 400
            Layout.maximumWidth: 400
            Layout.fillHeight: true
            padding: 0
            background: Rectangle {
                radius: 18
                color: "#16202a"
                border.color: "#24313f"
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 14

                SectionTitle { text: "Inspector" }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Button {
                        text: metaIsDownloading
                              ? "Downloading..."
                              : catalog.currentHasPreview
                                ? "Reload preview"
                                : metaCanDownload
                                  ? "Get local copy"
                                  : "No preview"
                        enabled: catalog.currentHasPreview ||
                                 metaCanDownload
                        onClicked: {
                            if (catalog.currentHasPreview)
                                schedulePreviewSourceRefresh(true)
                            else
                                catalog.downloadCurrent()
                        }
                    }

                    Button {
                        text: "Open marketplace"
                        enabled: String(currentMeta.sourceUrl || "").length > 0
                        onClicked: Qt.openUrlExternally(String(currentMeta.sourceUrl || ""))
                    }

                    Button {
                        text: "Asset info"
                        enabled: true
                        onClicked: tabs.currentIndex = 4
                    }

                    Item { Layout.fillWidth: true }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        text: riveView.playing ? "Pause" : "Play"
                        onClicked: riveView.playing ? riveView.pause() : riveView.play()
                    }

                    Button {
                        text: "Reload"
                        onClicked: riveView.reload()
                    }

                    Label {
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        color: riveView.status === RiveItem.Error ? "#ff9e9e" : "#8fa1b3"
                        text: riveView.status === RiveItem.Error ? riveView.errorString : "Ready"
                        wrapMode: Text.Wrap
                    }
                }

                TabBar {
                    id: tabs
                    Layout.fillWidth: true

                    TabButton { text: "Artboards" }
                    TabButton { text: "State Machines" }
                    TabButton { text: "Inputs" }
                    TabButton { text: "Events" }
                    TabButton { text: "Asset Info" }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: tabs.currentIndex

                    ListView {
                        clip: true
                        model: riveView.artboardsModel
                        spacing: 8
                        delegate: Button {
                            required property string name
                            required property bool current
                            width: ListView.view.width
                            text: current ? name + "  (current)" : name
                            highlighted: current
                            onClicked: riveView.selectArtboard(name)
                        }
                    }

                    ListView {
                        clip: true
                        model: riveView.stateMachinesModel
                        spacing: 8
                        delegate: Button {
                            required property string name
                            required property bool current
                            width: ListView.view.width
                            text: current ? name + "  (current)" : name
                            highlighted: current
                            onClicked: riveView.selectStateMachine(name)
                        }
                    }

                    Item {
                        clip: true

                        ListView {
                            id: inputsList
                            anchors.fill: parent
                            model: riveView.inputsModel
                            spacing: 10
                            delegate: Frame {
                                id: inputCard
                                required property string name
                                required property string path
                                required property string displayName
                                required property string kind
                                required property string source
                                required property var value
                                required property var minimum
                                required property var maximum
                                property bool usesViewModelApi: source === "ViewModel"

                                width: ListView.view.width
                                padding: 10
                                background: Rectangle {
                                    radius: 14
                                    color: "#1b2733"
                                    border.color: "#2d3c4c"
                                }

                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8

                                    Label {
                                        text: displayName
                                        color: "#eef4fa"
                                        font.pixelSize: 13
                                        font.bold: true
                                    }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8

                                        Label {
                                            text: kind
                                            color: "#8fa1b3"
                                            font.pixelSize: 11
                                        }

                                        Rectangle {
                                            radius: 999
                                            color: source === "ViewModel" ? "#27445a" : "#314429"
                                            implicitWidth: sourceLabel.implicitWidth + 12
                                            implicitHeight: sourceLabel.implicitHeight + 4

                                            Label {
                                                id: sourceLabel
                                                anchors.centerIn: parent
                                                text: source
                                                color: "#e7f2fb"
                                                font.pixelSize: 10
                                            }
                                        }

                                        Item { Layout.fillWidth: true }

                                        Label {
                                            visible: path !== displayName
                                            text: path
                                            color: "#6f8395"
                                            font.pixelSize: 10
                                        }
                                    }

                                    Button {
                                        visible: inputCard.kind === "Trigger"
                                        text: inputCard.usesViewModelApi ? "Fire View Model Trigger" : "Fire Trigger"
                                        onClicked: {
                                            if (inputCard.usesViewModelApi)
                                                riveView.fireViewModelTrigger(inputCard.path)
                                            else
                                                riveView.fireTrigger(inputCard.name)
                                        }
                                    }

                                    Switch {
                                        id: boolSwitch
                                        visible: inputCard.kind === "Boolean"
                                        Binding on checked {
                                            value: Boolean(inputCard.value)
                                            when: !boolSwitch.down
                                        }
                                        onToggled: {
                                            if (inputCard.usesViewModelApi)
                                                riveView.setViewModelValue(inputCard.path, checked)
                                            else
                                                riveView.setBoolean(inputCard.name, checked)
                                        }
                                    }

                                    ColumnLayout {
                                        visible: inputCard.kind === "Number"
                                        spacing: 6

                                        Slider {
                                            id: numberSlider
                                            Layout.fillWidth: true
                                            from: typeof inputCard.minimum === "number" ? inputCard.minimum : Math.min(-100, Number(inputCard.value))
                                            to: typeof inputCard.maximum === "number" ? inputCard.maximum : Math.max(100, Number(inputCard.value))
                                            Binding on value {
                                                value: Number(inputCard.value)
                                                when: !numberSlider.pressed
                                            }
                                            onMoved: {
                                                if (inputCard.usesViewModelApi)
                                                    riveView.setViewModelValue(inputCard.path, value)
                                                else
                                                    riveView.setNumber(inputCard.name, value)
                                            }
                                        }

                                        Label {
                                            text: Number(numberSlider.pressed ? numberSlider.value : inputCard.value).toFixed(2)
                                            color: "#c8d6e3"
                                            font.pixelSize: 12
                                        }
                                    }

                                    TextField {
                                        id: stringField
                                        visible: inputCard.kind === "String"
                                        Layout.fillWidth: true
                                        Binding on text {
                                            value: String(inputCard.value)
                                            when: !stringField.activeFocus
                                        }
                                        onEditingFinished: {
                                            if (inputCard.usesViewModelApi)
                                                riveView.setViewModelValue(inputCard.path, text)
                                            else
                                                riveView.setString(inputCard.name, text)
                                        }
                                    }
                                }
                            }
                        }

                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            visible: inputsList.count === 0

                            Label {
                                text: "Nothing to tweak here"
                                color: "#eef4fa"
                                font.pixelSize: 18
                                font.bold: true
                            }

                            Label {
                                width: 260
                                text: "The selected file is not exposing any state machine or view model controls right now."
                                color: "#93a6ba"
                                font.pixelSize: 12
                                wrapMode: Text.Wrap
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }

                    ColumnLayout {
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            SectionTitle { text: "Recent events" }
                            Item { Layout.fillWidth: true }
                            Button {
                                text: "Clear"
                                onClicked: riveView.clearEventLog()
                            }
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            model: riveView.eventsModel
                            spacing: 8

                            delegate: Frame {
                                required property string name
                                required property var payload
                                required property double timestamp
                                required property string sourceStateMachine
                                width: ListView.view.width
                                padding: 10
                                background: Rectangle {
                                    radius: 14
                                    color: "#1b2733"
                                    border.color: "#2d3c4c"
                                }

                                Column {
                                    anchors.fill: parent
                                    spacing: 4

                                    Label {
                                        text: name
                                        color: "#eef4fa"
                                        font.pixelSize: 13
                                        font.bold: true
                                    }

                                    Label {
                                        text: sourceStateMachine.length > 0 ? sourceStateMachine : "Unknown source"
                                        color: "#8fa1b3"
                                        font.pixelSize: 11
                                    }

                                    Label {
                                        width: parent.width
                                        text: JSON.stringify(payload)
                                        color: "#c8d6e3"
                                        font.pixelSize: 11
                                        wrapMode: Text.Wrap
                                    }
                                }
                            }
                        }
                    }

                    ScrollView {
                        clip: true

                        ColumnLayout {
                            width: parent.width
                            spacing: 10

                            SectionTitle {
                                text: String(currentMeta.title || "").length > 0 ? String(currentMeta.title) : "Nothing selected yet"
                            }

                            MetaRow { label: "Author"; value: String(currentMeta.author || "").length > 0 ? String(currentMeta.author) : "Unknown" }
                            MetaRow { label: "License"; value: String(currentMeta.license || "").length > 0 ? String(currentMeta.license) : "Unknown" }
                            MetaRow { label: "Availability"; value: String(currentMeta.availability || "").length > 0 ? String(currentMeta.availability) : "Unknown" }
                            MetaRow { label: "Preview"; value: catalog.currentHasPreview ? "Live" : "Poster only" }
                            MetaRow { label: "Cached file"; value: String(currentMeta.localFilePath || "").length > 0 ? String(currentMeta.localFilePath) : "Not downloaded" }
                            MetaRow { label: "For hire"; value: metaIsForHire ? "Yes" : "No" }
                            MetaRow { label: "Source"; value: String(currentMeta.sourceUrl || "").length > 0 ? String(currentMeta.sourceUrl) : "Unavailable" }

                            Label {
                                text: "Tags"
                                color: "#8fa1b3"
                                font.pixelSize: 12
                            }

                            Flow {
                                width: parent.width
                                spacing: 8

                                Repeater {
                                    model: currentMeta.tags || []
                                    delegate: Rectangle {
                                        required property var modelData
                                        radius: 999
                                        color: "#22303d"
                                        implicitWidth: tagText.implicitWidth + 12
                                        implicitHeight: tagText.implicitHeight + 4

                                        Label {
                                            id: tagText
                                            anchors.centerIn: parent
                                            text: modelData
                                            color: "#dce8f2"
                                            font.pixelSize: 10
                                        }
                                    }
                                }
                            }

                            Label {
                                text: "About"
                                color: "#8fa1b3"
                                font.pixelSize: 12
                            }

                            Label {
                                Layout.fillWidth: true
                                text: String(currentMeta.description || "")
                                color: "#eef4fa"
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }
            }
        }
    }
}
