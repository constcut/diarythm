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

        optionsRepeater.questionType = questionType
        optionsRepeater.model = 0
        optionsRepeater.model = optionsCount

        var optionsTexts = testsEngine.getOptionsTexts(qNum)

        for (var i = 0; i < optionsCount; ++i)
        {
            optionsRepeater.itemAt(i).text = optionsTexts[i]
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
                    id: optionsRepeater

                    property string questionType: ""


                    CheckBox {
                        id: checkOption
                        visible: optionsRepeater.questionType = "check"
                    }
                    RadioButton {
                        id: radioOption
                        visible: optionsRepeater.questionType = "radio"
                    }
                }
                //TEXT field vs repeater
            }

        }


    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
