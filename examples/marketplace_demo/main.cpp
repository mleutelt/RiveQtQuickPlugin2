#include <QDir>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "marketplacecatalog.h"

extern void qml_register_types_RiveQtQuick();

Q_LOGGING_CATEGORY(lcMarketplaceDemo, "rive.marketplaceDemo")

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");
    qml_register_types_RiveQtQuick();

    const QString sourceDir = QStringLiteral(RIVEQT_SOURCE_DIR);
    MarketplaceCatalogModel catalog;
    catalog.initialize(sourceDir);

    QQmlApplicationEngine engine;
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + "/qml");
    engine.addImportPath(appDir);
    QObject::connect(&engine,
                     &QQmlApplicationEngine::warnings,
                     &app,
                     [](const QList<QQmlError>& warnings) {
                         for (const QQmlError& warning : warnings)
                         {
                             qCWarning(lcMarketplaceDemo).noquote()
                                 << "qml warning:" << warning.toString();
                         }
                     });
    QObject::connect(&engine,
                     &QQmlApplicationEngine::objectCreationFailed,
                     &app,
                     [](const QUrl& url) {
                         qCWarning(lcMarketplaceDemo).noquote()
                             << "object creation failed for" << url.toString();
                     });
    engine.rootContext()->setContextProperty(QStringLiteral("catalog"), &catalog);
    engine.load(QUrl::fromLocalFile(QDir(appDir).absoluteFilePath("Main.qml")));
    if (engine.rootObjects().isEmpty())
    {
        qCWarning(lcMarketplaceDemo) << "failed to load Main.qml";
        return 1;
    }

    return app.exec();
}
