import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

ApplicationWindow{
    id: root
    width: 1200
    height: 600
    Material.primary: Material.Blue
    Material.accent: Material.Blue
    visible:true

    property AppSettings $settings: AppSettings{}

    ColumnLayout{
        spacing: 4
        RowLayout{
            Button{
                Layout.preferredWidth: 100
                Layout.preferredHeight: 40
                text: qsTr("停止")
                onClicked: {
                    player1.close()
                }
            }
            Button{
                Layout.preferredWidth: 100
                Layout.preferredHeight: 40
                text: qsTr("播放")
                highlighted: true
                onClicked: {
                    player1.start()
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
    }

}
