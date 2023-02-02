#include "player.h"
#include <QDebug>

HCPlayer::HCPlayer(QObject *parent)
    : QObject(parent){
    lUserID=-1;
    lRealPlayHandle=-1;
}

HCPlayer::~HCPlayer(){
    close();
}

QVideoSink *HCPlayer::videoSink() const{
    return m_videoSink.get();
}

void HCPlayer::setVideoSink(QVideoSink *newSink){
    if(m_videoSink == newSink) return;
    m_videoSink = newSink;
    emit videoSinkChanged();
}

// 全局绑定realhandle和lport
std::map<LONG, LONG> hpMap;
std::map<LONG, LONG> phMap;
// realhandle和player绑定
std::map<LONG, HCPlayer*> playerMap;

// 视频流数据解码回调
void CALLBACK dataDecCallback(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2){
//    qDebug() << "net dvr stream decode: 长度" << nSize << ", width:"<<pFrameInfo->nWidth<<" ,height:"<<pFrameInfo->nHeight<<" ,type:"<<pFrameInfo->nType;
    if(nSize<=0) return;
    QVideoFrame frame(QVideoFrameFormat(QSize(int(pFrameInfo->nWidth),int(pFrameInfo->nHeight)), QVideoFrameFormat::Format_YV12));
    // 检查有效和分配空间（mapped）
    if(!frame.isValid() || !frame.map(QVideoFrame::WriteOnly)){
//        emit errorMessage(QString("hc解码空间分配错误"));
        return;
    }
    memcpy(frame.bits(0), pBuf, nSize);
    frame.unmap();
    if(phMap.find(nPort)==phMap.end()){
//        emit errorMessage(QString("hc nPort无对应"));
        return;
    }
    LONG handler = phMap[nPort];
    if(playerMap.find(handler)==playerMap.end()){
//        emit errorMessage(QString("hc handler无对应"));
        return;
    }
    playerMap[handler]->m_videoSink->setVideoFrame(frame);
}
// 视频流数据回调
void CALLBACK dataCallback(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* dwUser){
    LONG lPort;
    switch(dwDataType){
    case NET_DVR_SYSHEAD:
        // 获取播放库未使用的通道号
        if (!PlayM4_GetPort(&lPort)) {
//            emit errorMessage(QString("hc 获取播放port失败"));
            break;
        }
        hpMap[lRealHandle] = lPort;
        phMap[lPort] = lRealHandle;
        qDebug() << "net dvr stream start:" << lRealHandle << "; "<<lPort;
        //第一次回调的是系统头，将获取的播放库 port 号赋值给全局 port，下次回调数据时即使用此 port 号播放
        if (dwBufSize > 0) {
            if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME)) {
                break;
            }
            //打开流接口
            if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024)){
                break;
            }
            PlayM4_SetDecCallBackMend(lPort,dataDecCallback,0);
            //播放开始
            if (!PlayM4_Play(lPort, NULL)){
//                emit errorMessage(QString("hc play失败"));
                break;
            }
        }
        break;
    case NET_DVR_STREAMDATA:
        if(hpMap.find(lRealHandle)==hpMap.end()){
//            emit errorMessage(QString("hc lRealHandle无对应"));
            break;
        }
        lPort = hpMap[lRealHandle];
        if (dwBufSize > 0){
            if (!PlayM4_InputData(lPort, pBuffer, dwBufSize)){
                break;
            }
        }
        break;
    }
}

void HCPlayer::start(const QString &ip, int port, const QString &username, const QString &pwd, int channel){
    if(lRealPlayHandle>=0){
        close();
    }
    bool ret;
    //初始化
    ret = NET_DVR_Init();
    QString err;
    if(!ret){
        err=QString::asprintf("hc init error:%lu",NET_DVR_GetLastError());
        qDebug()<<err;
        emit errorMessage(err);
        return;
    }
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    // 注册设备
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;
    lUserID = NET_DVR_Login_V30(ip.toStdString().data(), port, username.toStdString().data(), pwd.toStdString().data(), &struDeviceInfo);

    if (lUserID < 0){
        err=QString::asprintf("hc login error:%lu",NET_DVR_GetLastError());
        qDebug()<<err;
        emit errorMessage(err);
        NET_DVR_Cleanup();
        return;
    }
    //---------------------------------------
    //设置异常消息回调函数: 预览异常时重连
//    NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
    //---------------------------------------
    //启动预览
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
//    struPlayInfo.hPlayWnd = (HWND)w->winId();   //需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = channel;      //预览通道号
    struPlayInfo.dwStreamType = 0;  //0-主码流，1-子码流，2-码流 3，3-码流 4，以此类推
    struPlayInfo.dwLinkMode = 0;    //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;      //0- 非阻塞取流，1- 阻塞取流

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, dataCallback, NULL);
    if (lRealPlayHandle < 0){
        err=QString::asprintf("hc play error:%lu",NET_DVR_GetLastError());
        qDebug()<<err;
        emit errorMessage(err);
        return;
    }
    playerMap[lRealPlayHandle] = this;

    qDebug()<<"hc play success: "<<lRealPlayHandle;
}

void HCPlayer::close(){
    if(hpMap.find(lRealPlayHandle)!=hpMap.end()){
        LONG lPort = hpMap[lRealPlayHandle];
        PlayM4_Stop(lPort);
        PlayM4_CloseStream(lPort);
        PlayM4_FreePort(lPort);
        qDebug()<<"free playm4"<<lRealPlayHandle;
    }
    //关闭预览
    NET_DVR_StopRealPlay(lRealPlayHandle);
    //注销用户
    NET_DVR_Logout(lUserID);
    lUserID=-1;
    lRealPlayHandle=-1;
    //释放 SDK 资源
    NET_DVR_Cleanup();
}
