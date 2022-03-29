import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1

import QtQuick.Controls 1.4 as Quick1

Item {
    id: recorderItem

    function fillListWithRecords(records) {
        recordsModel.clear()

        for (var i = 0; i < records.length; ++i)
        {
            var record = records[i]
            recordsModel.append({"name":record[3], "date": record[0],
                                "time": record[1], "id": record[2]});
        }
    }


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

            Text {
                id: currentDateText
                text: "Date"
                visible: false
            }
        }

        RowLayout
        {
            spacing: 10

            Quick1.Calendar
            {
                id: calendar

                onSelectedDateChanged:
                {
                    currentDateText.text = selectedDate //Walkaround
                    currentDateText.text = currentDateText.text.substring(0, 10)
                    var records = sqlBase.findRecords(currentDateText.text)
                    recorderItem.fillListWithRecords(records)
                }
            }

            TextField {
                id: searchBox
                placeholderText: "Search string"

                onEditingFinished: {
                    search()
                }

                function search()
                {
                    var foundRecords = []

                    if (useDateInSearch.checked)
                    {
                        if (searchByName.checked)
                            foundRecords = sqlBase.findByNameMaskAndDate(
                                        currentDateText.text, searchBox.text)
                        else
                            foundRecords = sqlBase.findByTagMaskAndDate(
                                        currentDateText.text, searchBox.text)
                    }
                    else
                    {
                        if (searchByName.checked)
                            foundRecords = sqlBase.findByNameMask(searchBox.text)
                        else
                            foundRecords = sqlBase.findByTagMask(searchBox.text)
                    }

                    recorderItem.fillListWithRecords(foundRecords)
                }
            }

            RadioButton {
                id: searchByName
                checked: true
                text: "name"
            }

            RadioButton {
                id: searchByTag
                text: "tag"
            }

            CheckBox {
                id: useDateInSearch
                text: "Use date in search"
                checked: true
            }


            Button {
                text: "Search"
                onClicked: {
                    searchBox.search()
                }
            }

        }


        ListModel {
            id: recordsModel

            property string lastDate: ""
            property int lastLocalId: 0
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
                model: recordsModel
                Behavior on y { NumberAnimation{ duration: 200 } }
                onContentYChanged: {} //When implement search bar copy behavior
                delegate: recordDeligate
                highlight: highlightBar
                focus:  true
                ScrollBar.vertical: ScrollBar {}
            }
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
                    text: name + "   " + date + " " + time + " " + id
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
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    recordsModel.lastDate = date
                    recordsModel.lastLocalId = id

                    console.log("Last date", date, "last id", id)
                }
                onDoubleClicked: {
                    //Открыть окно единичной записи
                     mainWindow.requestSingleRecord(recordsModel.lastDate, recordsModel.lastLocalId);
                }
                onPressAndHold: {
                    //Меню
                    recorder.playFile(recordsModel.lastDate, recordsModel.lastLocalId)
                }
            }
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
