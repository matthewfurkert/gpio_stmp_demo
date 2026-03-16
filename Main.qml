import QtQuick
import QtQuick.Controls
import QtQuick.Window
import gpio_stmp_demo

ApplicationWindow {
    width: 400
    height: 300
    visible: true
    title: "GPIO HAL Hello World (Mock on desktop)"

    FontLoader {
        id: goRegular
        source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf")
    }

    Gpio {
        id: gpio1
        chip: 0
        pin: 14
        value: false
    }

    Pane {
        anchors.fill: parent
        background: Rectangle {
            color: "blue"
        }

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "GPIO Pin:" + gpio1.pin
                font.pixelSize: 24
            }
            Rectangle {
                width: 150
                height: 150
                radius: 75
                color: gpio1.value ? "#4ade80" : "#ef4444"
                border.color: "black"
                border.width: 4
                Text {
                    anchors.centerIn: parent
                    text: gpio1.value ? "ON" : "OFF"
                    font.pixelSize: 32
                    color: "white"
                }
            }
            Button {
                text: "TOGGLE GPIO"
                font.pixelSize: 18
                onClicked: gpio1.toggle()
            }
            Text {
                text: "Build type: " + (gpio1.isReal ? "REAL (libgpiod)" : "MOCK")
                font.pixelSize: 14
                color: "orange"
            }
        }
    }
}
