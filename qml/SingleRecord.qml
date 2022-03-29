import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1

import QtQuick.Controls 1.4 as Quick1

Item {
    id: singleRecordItem

    property string date: ""
    property int localIdx: 0

    ColumnLayout
    {
        x: 40
        y: 40

        spacing: 10

        Text {
            id: nameText
        }
        Text {
            id: dateTimeText
        }
        Text {
            id: durationMsText
        }

        TextField {
            id: tagsField
            placeholderText: "tags"
        }
        TextField {
            id: descriptionField
            placeholderText: "description"
        }
        Button {
            text: "Save"
            onClicked: {
                sqlBase.editAudioRecord(singleRecordItem.date, singleRecordItem.localIdx,
                                        tagsField.text, descriptionField.text)
            }
        }
        Button {
            text: "Play"
            onClicked: {
                recorder.playFile(singleRecordItem.date, singleRecordItem.localIdx)
            }
        }
    }


    Component.onCompleted:
    {
        var record =
            sqlBase.findSingleRecord(singleRecordItem.date,
                                     singleRecordItem.localIdx)

        dateTimeText.text = record[0] + " T " + record[1]
        nameText.text = record[3]
        durationMsText.text = record[4]

        tagsField.text = record[5]
        descriptionField.text = record[6]
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}