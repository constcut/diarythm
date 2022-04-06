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
        var cardRecords

        if (useDateCheck.checked)
        {
            cardRecords = sqlBase.getAllCardRecordsOnDate(cardDateText.text)
            console.log("All card records on date", cardRecords)
            console.log("And date is: ", cardDateText.text)

            //+ option selected card or not
        }
        else
        {
            cardRecords = sqlBase.getAllCardRecords();
            console.log("All card records ", cardRecords)

            //+ option selected card or not
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

            CheckBox {
                id: useCardCheck
                text: "Use card"
                checked: true

                onCheckedChanged: cardRecordsItem.loadResults()
            }

            ComboBox {
                id: allCards
                implicitWidth: 150

                onCurrentTextChanged: {

                    if (useCardCheck.checked)
                        cardRecordsItem.loadResults()
                }
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

            RoundButton {
                text: "Back to cards"
                onClicked: mainWindow.requestDiaryCard()
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
