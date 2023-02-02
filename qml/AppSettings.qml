import QtQuick
import Qt.labs.settings

Settings{
    id:setting
    fileName:"./setting.ini"

//    property var home_path
    property int width: 1100
    property int height: 800

    // nvr
    property string nvr_username: "<your-username>"
    property string nvr_pwd: "<your-pwd>"
    property string nvr_ip: "<your-ip>"
    property int nvr_port: 8000
}
