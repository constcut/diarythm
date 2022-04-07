import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtWebKit 3.0

Item {

    id: pageItem


    Component.onCompleted:
    {

    }


    ColumnLayout
    {
        x: 25
        y: 25
        spacing: 10

        RowLayout
        {
            spacing:  10

            RoundButton {
                text: "just"
            }
        }

        WebView {
            width: 900
            height: 400
        }

    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
