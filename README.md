# HCPlayer Demo

基于海康威视SDK（C++ windows）、Qt6.4.2、QML、CMake，实现摄像头或NVR在QML中的实时预览。

大致原理：
- 通过海康SDK登录设备，获取视频流
- 用海康SDK中的播放器SDK(PlayM4)解码视频流，解码后的数据为YV12格式的图像数据
- 将图像数据通过QVideoSink接口传递给QML层的VideoOutput

注：Qt5使用QAbstractVideoSurface类，和Qt6完全不同。