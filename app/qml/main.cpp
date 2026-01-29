#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char* argv[])
{
	// Use Basic style to allow full button customization
	QQuickStyle::setStyle("Basic");

	QGuiApplication app(argc, argv);

	// Parse --deck argument
	QString deckName;
	QStringList args = app.arguments();
	for (int i = 1; i < args.size(); ++i) {
		if (args[i] == "--deck" && i + 1 < args.size()) {
			deckName = args[++i];
		}
	}

	QQmlApplicationEngine engine;

	// Expose deck name to QML
	engine.rootContext()->setContextProperty("initialDeckName", deckName);

	// Add qml folder next to executable to import path
	QString appDir = QCoreApplication::applicationDirPath();
	engine.addImportPath(appDir + "/qml");

	engine.load(QUrl(QStringLiteral("qrc:/blackjack/Main.qml")));

	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
