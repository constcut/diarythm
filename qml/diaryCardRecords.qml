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
            if (useCardCheck.checked)
                cardRecords = sqlBase.getAllCardRecordsForCardAndDate(
                              allCards.currentText, cardDateText.text)
            else
               cardRecords = sqlBase.getAllCardRecordsOnDate(cardDateText.text)
        }
        else
        {
            if (useCardCheck.checked)
                cardRecords = sqlBase.getAllCardRecordsForCard(allCards.currentText)
            else
                cardRecords = sqlBase.getAllCardRecords();

        }

        resultsRepeater.model = 0
        resultsRepeater.model = cardRecords.length

        for (var i = 0; i < cardRecords.length; ++i)
            if (resultsRepeater.itemAt(i) !== null)
                resultsRepeater.itemAt(i).loadValues(cardRecords[i])

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

                if (useCardCheck.checked)
                    cardRecordsItem.loadResults()
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

                onCheckedChanged: cardRecordsItem.loadResults()
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
                function loadValues(cardRecord)
                {
                    dateText.text = cardRecord[0]
                    timeText.text = cardRecord[1]
                    //cardRecord[2] - local id group inputed at the same time
                    //cardRecord[3] - card id
                    //cardRecord[4] - card date
                    //cardRecord[5] - group name
                    fieldNameText.text = cardRecord[6]
                    fieldValueText.text = cardRecord[7]
                    fieldText.text = cardRecord[8]
                }

                Text {
                    text: "Date:"
                }
                Text {
                    id: dateText
                }
                Text {
                    text: "Time:"
                }
                Text {
                    id: timeText
                }
                Text {
                    text: "Field name:"
                }
                Text {
                    id: fieldNameText
                }
                Text {
                    text: "Value:"
                }
                Text {
                    id: fieldValueText
                }
                Text {
                    id: fieldText
                }
            }
        }


    } // main ColumnLayout


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
