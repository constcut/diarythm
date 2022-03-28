import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1

Item {
    id: audioHandlerItem

    RowLayout {
        spacing: 10


        ToolButton {
            //property bool recording: false
            text: "Start" //recording ? "Start" : "Pause"
            onClicked: {
                //recording = !recording
                recorder.start()
            }
        }

        ToolButton {
            text: "Stop and save"
            onClicked:  {
                recorder.stop()
            }
        }

        ToolButton {
            text: "Pause"
            onClicked:  {
                recorder.pause()
            }
        }

        ToolButton {
            text: "Cancel"
            onClicked: {
                recorder.cancel()
            }
        }

        ToolButton {
            text: "Settings"
        }

        Text {
            id: dbsText
            text: "Volume lvl"
        }

        Text {
            id: timeText
            text: "Time"
        }
    }

    Connections {
        target: recorder

        function onTimeUpdate(ms) {
            timeText.text = ms
        }

        function onDbsUpdate(dbs) {
            dbsText.text = dbs
        }
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
