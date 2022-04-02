import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1

Item {

    id: diaryCardItem

    function updateFields()
    {
        var allFields = cardEngine.getAllGroupFields(groupsNames.currentText)
        fieldsRepeater.model = 0
        fieldsNames.model = allFields
        fieldsRepeater.fieldsNames = allFields
        fieldsRepeater.model = allFields.length
    }


    Component.onCompleted:
    {
        cardName.text = cardEngine.getCardName()
        cardDescription.text =  cardEngine.getCardDescription()
        groupsNames.model = cardEngine.getAllGroupsNames()

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

            Text {
                text: "Date: "
            }
            Text {
                id: dateText
            }
            RoundButton {
                text: ".."
            }
            Button {
                text: "Add values"
                //Add only not empty, maybe use localId for the same group of fields edited once
            }
        }


        IntValidator {
            id: intValidator
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

                    //Compnent on completed to avoid

                    Component.onCompleted: {

                        var fieldName = fieldsRepeater.fieldsNames[index]
                        var groupName = groupsNames.currentText

                        var fieldType = cardEngine.getFieldType(groupName, fieldName)

                        textInfo.text = fieldName

                        textField.visible = (fieldType === "text") || (fieldType === "int") || (fieldType === "real")
                        comboField.visible = (fieldType === "range") || (fieldType === "enum")
                        checkField.visible = fieldType === "bool"

                        if (textField.visible)
                            textField.placeholderText = fieldName

                        if (checkField.visible)
                            checkField.text = fieldName

                        if (fieldType === "int")
                            textField.validator = intValidator

                        if (fieldType === "real")
                            textField.inputMethodHints = Qt.ImhFormattedNumbersOnly

                        if (fieldType === "range")
                        {
                            var rangeMin = cardEngine.getFieldRangeMin(groupName, fieldName)
                            var rangeMax = cardEngine.getFieldRangeMax(groupName, fieldName)

                            var rangeList = []

                            for (var i = rangeMin; i <= rangeMax; ++i)
                                rangeList.push(i)

                            comboField.model = rangeList
                        }

                        if (fieldType === "enum")
                        {

                            var enumName = cardEngine.getFieldEnum(groupName, fieldName)
                            var displayNames = cardEngine.getEnumDisplayNames(enumName)

                            var allEnums = cardEngine.getAllEnumsNames()

                            console.log("ENUM type", enumName)
                            console.log(displayNames)
                            console.log("All enums ", allEnums)



                            comboField.model = displayNames
                        }

                        //bool - checkbox
                        //range, enum - ComboBox
                    }

                    Text
                    {
                        y: 10
                        id: textInfo
                    }

                    TextField
                    {
                        id: textField
                        x: textInfo.width + 10

                    }
                    ComboBox {
                        id: comboField
                        x: textInfo.width + 10
                    }
                    CheckBox {
                        id: checkField
                        x: textInfo.width + 10
                    }
                }

            } //Repater

        } //RowLayout
    }


    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
