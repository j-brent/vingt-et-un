#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() = default;

public slots:
    void qmlEngineAvailable(QQmlEngine* engine)
    {
        engine->addImportPath(QStringLiteral("qrc:/"));
    }
};

QUICK_TEST_MAIN_WITH_SETUP(blackjack_qml_tests, Setup)

#include "tst_main.moc"
