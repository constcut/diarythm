import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1

Item {
    id: recorderItem


    ColumnLayout
    {
        spacing: 10

        RowLayout
        {
            spacing: 10

            ToolButton {
                //property bool recording: false
                text: "Start" //recording ? "Start" : "Pause"
                onClicked: {
                    //recording = !recording
                    connector.started = false
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
                text: "Time ms"
            }

            Button {
                text: "Back to calendar"
                onClicked: {
                    mainWindow.requestCalendar()
                }
            }
        }

    }


    Connections {

        id: connector
        property bool started: false

        target: recorder

        function onTimeUpdate(ms) {
            timeText.text = ms
            if (connector.started === false) {
                connector.started = true
                console.log("Started recording!")
            }
        }

        function onDbsUpdate(dbs) {
            dbsText.text = dbs
        }
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
