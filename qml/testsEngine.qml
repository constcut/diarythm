import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Item {

    id: testsEngineItem

    Component.onCompleted:
    {
        testName.text = testsEngine.getName()
        questionsCount.text = testsEngine.questionsCount()
    }


    ColumnLayout
    {
        spacing: 10
        y: 40
        x: 40


        RowLayout
        {
            spacing: 10

            Text {
                text: "Test name: "
            }
            Text {
                id: testName
            }
            Text {
                text: "Questions count: "
            }
            Text {
                id: questionsCount
            }

        }
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
