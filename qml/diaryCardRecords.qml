import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Item {

    id: cardRecordsItem

    Component.onCompleted:
    {
        loadResults()
    }

    function loadResults()
    {

    }

    ColumnLayout
    {
        spacing: 10
        y: 40
        x: 40


        RowLayout
        {
            spacing: 10

            ComboBox {
                id: allCards
                implicitWidth: 150

                onCurrentTextChanged: {
                    cardRecordsItem.loadResults()
                }
            }

            RoundButton {
                text: "Back to cards"
                onClicked: {}
            }
        }

        Repeater
        {
            id: resultsRepeater

            RowLayout
            {
                function loadValues(date, time)
                {
                    dateText.text = date
                    timeText.text = time
                }


                Text {
                    text: "Date: "
                }
                Text {
                    id: dateText
                }
                Text {
                    text: "Time: "
                }
                Text {
                    id: timeText
                }
            }
        }


    } // main ColumnLayout


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
