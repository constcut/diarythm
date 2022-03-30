import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1

Item {
    id: textRecordsItem

    ColumnLayout
    {
        x: 40
        y: 40

        spacing: 10

        RowLayout {

            spacing: 10

            TextField {
                id: nameField
                placeholderText: "Name"
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
                text: "Save text"
                onClicked: {
                    sqlBase.addText(nameField.text, textArea.text, tagsField.text,
                                    descriptionField.text)

                    nameField.text = "" //Защита от повторного добавления
                    tagsField.text = ""
                    descriptionField.text = ""
                    textArea.text = ""
                    //Возможно стоит переключать на другой экран
                }
            }
        }

        ScrollView {
            id: view
            width: 650
            clip: true

            implicitWidth: 650

            TextArea {

                background: Rectangle {
                    //color: "lightgreen"
                    border.color: "lightgreen"
                }

                implicitWidth: parent.width

                id: textArea
                text: ""
                placeholderText: "Input text here"
            }

            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        }
    }



    function keyboardEventSend(key, mode) {
        //Заглушка обработки клавиш
    }

}
