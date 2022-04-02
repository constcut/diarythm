import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1

Item {

    id: diaryCardItem

    function updateFields()
    {
        fieldsNames.model = cardEngine.getAllGroupFields(groupsNames.currentText)
        var allFields = cardEngine.getAllGroupFields(groupsNames.currentText)
        fieldsRepeater.fieldsNames = allFields
        fieldsRepeater.model = allFields.length
    }


    Component.onCompleted:
    {
        cardName.text = cardEngine.getCardName()
        cardDescription.text =  cardEngine.getCardDescription()
        groupsNames.model = cardEngine.getAllGroupsNames()
        groupsNames.currentIndex = 0

        updateFields()
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
                id: cardName
            }

            Text {
                id: cardDescription
            }

            Text {
                text: "Group: "
            }

            ComboBox {
                id: groupsNames
                implicitWidth: 200

                onCurrentTextChanged: {
                    diaryCardItem.updateFields()
                }
            }

            Text {
                text: "Field: "
            }

            ComboBox {
                id: fieldsNames
            }
        }

        RowLayout
        {

            Repeater
            {
                id: fieldsRepeater

                property var fieldsNames: []

                Rectangle
                {
                    width: textInfo.width + 10 + textField.width
                    height: 40

                    Text
                    {
                        y: 10
                        id: textInfo
                        text: fieldsRepeater.fieldsNames[index]
                    }

                    TextField
                    {
                        id: textField
                        x: textInfo.width + 10
                        placeholderText: fieldsRepeater.fieldsNames[index]
                    }
                }

            } //Repater

        } //RowLayout
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
