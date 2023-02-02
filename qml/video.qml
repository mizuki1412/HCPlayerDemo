import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtMultimedia

ApplicationWindow{
    id:videoPage
    width: 1200
    height: 800
    visible: false
    Material.primary: Material.Blue
    Material.accent: Material.Blue

//    flags: Qt.Window | Qt.FramelessWindowHint
    ColumnLayout{
        spacing: 4
        RowLayout{
            AppButton{
                size: 18
                Layout.preferredWidth: 60
                Layout.preferredHeight: 30
                text: qsTr("close")
                onClicked: {
                    player1.close()
                    player2.close()
                }
            }
            AppButton{
                size: 18
                Layout.preferredWidth: 80
                Layout.preferredHeight: 30
                text: qsTr("restart")
                onClicked: {
                    player1.restart()
                    player2.restart()
                }
            }
        }

        KitHCPlayer{
            id: player1
            Layout.preferredWidth: 1920*480/1080
            Layout.preferredHeight: 480
            username: $settings.nvr_username
            pwd: $settings.nvr_pwd
            ip: $settings.nvr_ip
            port: $settings.nvr_port
            channel: 1
        }
        KitHCPlayer{
            id: player2
            Layout.preferredWidth: 1920*480/1080
            Layout.preferredHeight: 480
            username: $settings.nvr_username
            pwd: $settings.nvr_pwd
            ip: $settings.nvr_ip
            port: $settings.nvr_port
            channel: 1
        }
    }

}

