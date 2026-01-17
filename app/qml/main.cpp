#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    // Use Basic style to allow full button customization
    QQuickStyle::setStyle("Basic");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Add qml folder next to executable to import path
    QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + "/qml");

    engine.load(QUrl(QStringLiteral("qrc:/blackjack/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
