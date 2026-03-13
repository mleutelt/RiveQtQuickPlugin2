#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

extern void qml_register_types_RiveQtQuick();

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    qml_register_types_RiveQtQuick();

    const QString sourceDir = QStringLiteral(RIVEQT_SOURCE_DIR);
    const QUrl assetUrl = QUrl::fromLocalFile(
        QDir(sourceDir).absoluteFilePath(
            QStringLiteral("tests/assets/rive/hosted_image_file.riv")));

    QQmlApplicationEngine engine;
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + "/qml");
    engine.addImportPath(appDir);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleRiveUrl"),
                                             assetUrl);
    engine.load(QUrl::fromLocalFile(QDir(appDir).absoluteFilePath("Main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        return 1;
    }

    return app.exec();
}
