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
        var questionType = testsEngine.getQuestionType(qNum)

        checksRepeater.model = 0
        radioRepeater.model = 0
        questionField.visible = false

        var optionsTexts = testsEngine.getOptionsTexts(qNum)
        var i = 0

        if (questionType === "radio")
        {
            radioRepeater.model = optionsCount

            for (i = 0; i < optionsCount; ++i)
                radioRepeater.itemAt(i).text = optionsTexts[i]

        }
        else if (questionType === "check")
        {
            checksRepeater.model = optionsCount

            for (i = 0; i < optionsCount; ++i)
                checksRepeater.itemAt(i).text = optionsTexts[i]
        }
        else if (questionType === "text")
        {
            questionField.visible = true
        }

    }

    function loadNextQuestion()
    {
        var qCount = testsEngine.questionsCount()
        if (testsEngineItem.questionNumber + 1 < qCount)
        {
            testsEngineItem.questionNumber += 1
            testsEngineItem.loadQuestion()
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
                text: "Next"
                onClicked: testsEngineItem.loadNextQuestion()
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

                y: 10
                x: 20

                Text {
                    id: questionNumber
                }
                Text {
                    id: questionText
                }

                Repeater
                {
                    id: checksRepeater
                    CheckBox {
                        id: checkOption
                    }
                }
                Repeater
                {
                    id: radioRepeater
                    RadioButton {
                        id: radioOption
                    }
                }
                TextField
                {
                    id: questionField
                    placeholderText: "Answer"
                }

                //TEXT field vs repeater
            }

        }


    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
