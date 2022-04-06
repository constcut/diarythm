import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Item {

    id: testsResulstsItem

    Component.onCompleted:
    {
        allTests.model = sqlBase.getAllTestsNames()
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

        ComboBox {
            id: allTests
            implicitWidth: 150

            onCurrentTextChanged: {
                testsResulstsItem.loadResults()
            }
        }


    } // main ColumnLayout


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
