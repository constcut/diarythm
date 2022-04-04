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
        loadQuestion()
    }

    property int questionNumber: 0

    function loadQuestion()
    {
        var qNum = testsEngineItem.questionNumber
        questionNumber.text = qNum
        questionText.text = testsEngine.getQuestionText(qNum)

        var optionsCount = testsEngine.getOptionsCount(qNum)
        optionsRepeater.model = optionsCount

        for (var i = 0; i < optionsCount; ++i)
        {
            optionsRepeater.itemAt(i).text = i
        }
    }

    function loadNextQuestion()
    {
        testsEngineItem.questionNumber += 1
        testsEngineItem.loadQuestion()
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
            RoundButton {
                text: "Start"
            }
        }

        Rectangle
        {
            width: 700
            height: 400
            border.color: "lightgreen"

            ColumnLayout
            {
                spacing:  20

                Text {
                    id: questionNumber
                }
                Text {
                    id: questionText
                }

                Repeater {
                    id: optionsRepeater

                    Text {
                        id: option
                    }
                }
            }

        }


    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
