import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import HCPlayer

Rectangle{
    id: rect
    color: "black"
    property string username
    property string pwd
    property string ip
    property int port
    property int channel
    property bool autoStart:true

    signal errHandle(var msg)

    Component.onCompleted: {
        if(autoStart) start()
    }
    function start(){
        player.start(ip, port, username, pwd, channel)
    }
    function close(){
        player.close()
    }
    function restart(){
        close()
        start()
    }

    VideoOutput{
        id: videoOutput
        anchors.fill: parent
        MouseArea{
            anchors.fill: parent
            onDoubleClicked: {
                rect.restart()
            }
        }
    }
    HCPlayer{
        id: player
        videoSink: videoOutput.videoSink
        onErrorMessage: (msg)=>{
            rect.errHandle(msg)
        }
    }
}
