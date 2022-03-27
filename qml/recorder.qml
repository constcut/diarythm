import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1

Item {
    id: audioHandlerItem

    RowLayout {
        spacing: 10
        ToolButton {
            property bool recording: false
            text: recording ? "Start" : "Pause"
            onClicked: {
                recording = !recording
            }
        }

        ToolButton {
            text: "Stop and save"
        }


        ToolButton {
            text: "Stop and clear"
        }

        ToolButton {
            text: "Settings"
        }

        Text {
            text: "Volume lvl"
        }

        Text {
            text: "Time"
        }
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
