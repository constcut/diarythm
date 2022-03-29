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

                }
            }
        }

        ScrollView {
            id: view
            width: textRecordsItem.width - 10
            height: textRecordsItem.height - 100
            clip: true

            implicitWidth: 650

            TextArea {

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
