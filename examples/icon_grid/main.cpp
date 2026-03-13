#include <QDir>
#include <QGuiApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "icongridmodel.h"

extern void qml_register_types_RiveQtQuick();

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");
    qml_register_types_RiveQtQuick();

    const QString sourceDir = QStringLiteral(RIVEQT_SOURCE_DIR);
    const QUrl assetUrl = QUrl::fromLocalFile(
        QDir(sourceDir).absoluteFilePath(
            QStringLiteral("tests/assets/rive/25691-49048-interactive-icon-set.riv")));

    IconGridModel iconGridModel;

    QQmlApplicationEngine engine;
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + "/qml");
    engine.addImportPath(appDir);
    engine.rootContext()->setContextProperty("exampleRiveUrl", assetUrl);
    engine.rootContext()->setContextProperty("iconGridModel", &iconGridModel);
    engine.rootContext()->setContextProperty("creditTitle", "Interactive Icon Set");
    engine.rootContext()->setContextProperty("creditAuthor", "gabermonti");
    engine.rootContext()->setContextProperty("creditLicense", "CC BY 4.0");
    engine.rootContext()->setContextProperty("creditStatus", "For Hire");
    engine.rootContext()->setContextProperty(
        "creditUrl",
        QUrl("https://rive.app/marketplace/25691-49048-interactive-icon-set/"));
    engine.load(QUrl::fromLocalFile(QDir(appDir).absoluteFilePath("Main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        return 1;
    }

    QTimer::singleShot(0, &iconGridModel, [assetUrl, &iconGridModel]() {
        iconGridModel.setSource(assetUrl);
    });

    return app.exec();
}
