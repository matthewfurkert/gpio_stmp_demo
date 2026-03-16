import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import gpio_stmp_demo

ApplicationWindow {
    readonly property bool embeddedDev: Qt.platform.pluginName === "eglfs"

    id: window
    width: embeddedDev ? 480 : 800
    height: embeddedDev ? 800 : 480
    visible: true
    title: "GPIO HAL Hello World"

    FontLoader {
        id: goRegular
        source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf")
    }

    Gpio {id: gpioBoard; chip: 0; pin: 14; value: false}
    Gpio {id: gpioRed; chip: 5; pin: 4; value: false}
    Gpio {id: gpioOrange; chip: 5; pin: 6; value: false}
    Gpio {id: gpioGreen; chip: 5; pin: 3; value: false}

    Pane {
        anchors.fill: parent
        width: embeddedDev ? window.height : window.width
        height: embeddedDev ? window.width : window.height
        rotation: embeddedDev ? 270 : 0
        background: Rectangle {
            color: "grey"
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.leftMargin: 80
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20

            CheckBox {
                id: checkRed
                checked: gpioRed.value
                onClicked: gpioRed.toggle()
                indicator: Rectangle {
                    implicitWidth: 72; implicitHeight: 72; radius: 6
                    border.color: "red"
                    color: checkRed.checked ? "red" : "white"
                }
                contentItem: Column { leftPadding: 72; spacing: 2
                    Text { text: qsTr("Red"); color: "red"; font.pixelSize: 24 }
                    Text { text: qsTr("(PF4)"); color: "red"; font.pixelSize: 20 }
                }
            }

            CheckBox {
                id: checkOrange
                checked: gpioOrange.value
                onClicked: gpioOrange.toggle()
                indicator: Rectangle {
                    implicitWidth: 72; implicitHeight: 72; radius: 6
                    border.color: "orange"
                    color: checkOrange.checked ? "orange" : "white"
                }
                contentItem: Column { leftPadding: 72; spacing: 2
                    Text { text: qsTr("Orange"); color: "orange"; font.pixelSize: 24 }
                    Text { text: qsTr("(PF6)"); color: "orange"; font.pixelSize: 20 }
                }
            }

            CheckBox {
                id: checkGreen
                checked: gpioGreen.value
                onClicked: gpioGreen.toggle()
                indicator: Rectangle {
                    implicitWidth: 72; implicitHeight: 72; radius: 6
                    border.color: "green"
                    color: checkGreen.checked ? "green" : "white"
                }
                contentItem: Column { leftPadding: 72; spacing: 2
                    Text { text: qsTr("Green"); color: "green"; font.pixelSize: 24 }
                    Text { text: qsTr("(PF3)"); color: "green"; font.pixelSize: 20 }
                }
            }
        }
        ColumnLayout {
            anchors.right: parent.right
            anchors.top: parent.top
            width: parent.width * 0.5
            height: parent.height * 0.75

            Item { Layout.preferredHeight: 50 }

            RowLayout {
                spacing: 25
                Layout.alignment: Qt.AlignHCenter
                ColumnLayout {
                    spacing: 20
                    Text {
                            text: qsTr("OFF / ON"); color: "Blue"; font.pixelSize: 24
                            Layout.alignment: Qt.AlignHCenter
                        }
                    Switch {
                        id: boardLed
                        scale: 3
                        Layout.alignment: Qt.AlignHCenter
                        checked: gpioBoard.value
                        onCheckedChanged: {
                            if (!checked) {
                                gpioBoard.toggle()
                            }
                        }
                    }
                }
                ColumnLayout {
                    Layout.alignment: Qt.AlignVCenter
                    Item { Layout.preferredHeight: 50 }
                    Text {
                        text: qsTr("Board Led")
                        color: "Blue"; font.pixelSize: 24
                        verticalAlignment: Text.AlignBottom
                    }
                    Text {
                        text: qsTr("(PA14)")
                        color: "Blue"; font.pixelSize: 20
                        verticalAlignment: Text.AlignBottom
                    }
                }
            }
            Button {
                id: lightShowButton
                text: qsTr("Light Show")
                Layout.alignment: Qt.AlignHCenter

                font.pixelSize: 26
                font.bold: true
                padding: 22

                background: Rectangle {
                    implicitWidth: 280
                    implicitHeight: 85
                    radius: 16
                    color: enabled ? (parent.down ? "#1e40af" : "#2563eb") : "#94a3b8"
                    border.color: "#1e40af"
                    border.width: 3
                }

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (text === "Light Show") {
                        // boardLed.on
                        text = "Stop Show"
                    } else {
                        text = "Light Show"
                    }
                }
            }
            Text {
                text: "Build type: " + (gpioBoard.isReal ? "REAL (libgpiod)" : "MOCK")
                font.pixelSize: 20; color: "orange"; Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
