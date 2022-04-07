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

        rowsModel.clear()

        for (var i = 0; i < cardRecords.length; ++i)
        {
            if (resultsRepeater.itemAt(i) !== null)
                resultsRepeater.itemAt(i).loadValues(cardRecords[i])

            rowsModel.append({"datePart":cardRecords[i][0], "timePart":cardRecords[i][1], "localId":cardRecords[i][2],
                             "cardId":cardRecords[i][3], "cardDate":cardRecords[i][4], "groupName":cardRecords[i][5],
                             "fieldName":cardRecords[i][6], "fieldValue":cardRecords[i][7], "fieldText":cardRecords[i][8]})
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

        ListModel {
            id: rowsModel
        }

        Rectangle
        {
            id: listViewRectangle
            width: 600
            height: 300
            ListView
            {
                id: recordsList
                clip: true
                anchors.fill: parent
                model: rowsModel
                Behavior on y { NumberAnimation{ duration: 200 } }
                onContentYChanged: {} //When implement search bar copy behavior
                delegate: recordDeligate
                highlight: highlightBar
                focus:  true
                ScrollBar.vertical: ScrollBar {}
            }
        }


        Component {
            id: highlightBar
            Rectangle {
                id: highlightBarRect
                width: 200; height: 50
                color: "#88FF88"
                y: recordsList.currentItem == null ? 0 : recordsList.currentItem.y
                Behavior on y { SpringAnimation { spring: 2; damping: 0.3 } }
            }
        }



        Component {
            id: recordDeligate
            Item {
                id: wrapper
                width: recordsList.width
                height: 35
                Column {
                    Text {
                        text: ""
                    }
                }
                states: State {
                    name: "Current"
                    when: wrapper.ListView.isCurrentItem
                    PropertyChanges { target: wrapper; x: 20 }
                }
                transitions: Transition {
                    NumberAnimation { properties: "x"; duration: 200 }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {
                        wrapper.ListView.view.currentIndex = index
                    }
                }
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
