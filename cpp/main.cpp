#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuickControls2>
#include "hk/player.h"

int main ( int argc, char *argv[] ){
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    qmlRegisterType<HCPlayer> ( "HCPlayer", 1, 0, "HCPlayer" );

    const QUrl url(u"qrc:///qml/root.qml"_qs);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

    return app.exec();
}
