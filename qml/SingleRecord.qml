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
    }


    Component.onCompleted:
    {
        var record =
            sqlBase.findSingleRecord(singleRecordItem.date,
                                     singleRecordItem.localIdx)

        dateTimeText.text = record[0] + " T " + record[1]
        nameText.text = record[2]
        durationMsText.text = record[3]
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
