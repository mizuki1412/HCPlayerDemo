#ifndef HC_PLAYER_H
#define HC_PLAYER_H
#include <QObject>
#include <QVideoSink>
#include <QVideoFrameFormat>
#include <QVideoFrame>
#include <QPointer>
#include <QQmlEngine>
#include "HCNetSDK.h"
#include "DataType.h"
#include "plaympeg4.h"
#include "DecodeCardSdk.h"


class HCPlayer : public QObject{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)

public:
    HCPlayer(QObject *parent = nullptr);
    ~HCPlayer();

    QVideoSink *videoSink() const;
    // 由qml层赋值
    void setVideoSink(QVideoSink *newSink);

    Q_INVOKABLE void start(const QString &ip, int port, const QString &username, const QString &pwd, int channel);
    Q_INVOKABLE void close();

    QPointer<QVideoSink> m_videoSink;

private:
    // 句柄
    LONG lRealPlayHandle;
    LONG lUserID;

signals:
    void videoSinkChanged();
    // 错误消息信号
    void errorMessage(const QString msg);

};

#endif // HC_PLAYER_H
