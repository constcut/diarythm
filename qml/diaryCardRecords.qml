import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Item {

    id: cardRecordsItem

    Component.onCompleted:
    {
        allCards.model = sqlBase.getAllCardsNames()
        loadResults()
    }

    function loadResults()
    {
        if (useDateCheck.checked)
        {
            var cardRecords = sqlBase.getAllCardRecordsOnDate()
            console.log("All card records on date", cardRecords)
        }
    }


    Dialog
    {
        id: calendarDialog

        width: 400
        height: 400

        Quick1.Calendar
        {
            anchors.fill: parent

            id: calendar

            onSelectedDateChanged:
            {
                cardDateText.text = selectedDate
                cardDateText.text = cardDateText.text.substring(0, 10)
            }
        }
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
                onClicked: mainWindow.requestDiaryCard()
            }

            CheckBox {
                id: useDateCheck
                text: "Use date"
                checked: true
            }

            Text {
                text: "Card date: "
            }

            Text {
                id: cardDateText
            }

            RoundButton {
                text: ".."
                onClicked: calendarDialog.open()
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
