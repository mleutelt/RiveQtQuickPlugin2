#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

#include <QFileInfo>
#include <QString>

#include "../../src/RiveQtQuick/private/riveinspector.h"

class SourceLoadingTest : public QObject
{
    Q_OBJECT

private slots:
    void checkedInAssetsExist()
    {
        const QString assetDir = QStringLiteral(RIVEQT_TEST_ASSET_DIR);
        const QString hello = assetDir + "/hello_world.riv";
        const QString triggers = assetDir + "/state_machine_triggers.riv";
        const QString binding = assetDir + "/data_binding_test.riv";
        const QString hosted = assetDir + "/hosted_image_file.riv";
        const QString hostedPayload =
            QStringLiteral(RIVEQT_TEST_HOSTED_ASSET_DIR) + "/one-45008.png";

        QVERIFY2(QFileInfo::exists(hello), qPrintable(hello));
        QVERIFY2(QFileInfo::exists(triggers), qPrintable(triggers));
        QVERIFY2(QFileInfo::exists(binding), qPrintable(binding));
        QVERIFY2(QFileInfo::exists(hosted), qPrintable(hosted));
        QVERIFY2(QFileInfo::exists(hostedPayload), qPrintable(hostedPayload));
    }

    void inputModelUpdatesInPlaceForStableRows()
    {
        RiveInputListModel model;
        const QVector<RiveInputSnapshot> initialEntries{
            {.name = QStringLiteral("speed"),
             .path = QStringLiteral("speed"),
             .displayName = QStringLiteral("speed"),
             .kind = QStringLiteral("Number"),
             .source = QStringLiteral("ViewModel"),
             .value = 1.0},
            {.name = QStringLiteral("enabled"),
             .path = QStringLiteral("enabled"),
             .displayName = QStringLiteral("enabled"),
             .kind = QStringLiteral("Boolean"),
             .source = QStringLiteral("ViewModel"),
             .value = true},
        };

        model.setEntries(initialEntries);

        QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
        QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

        const QVector<RiveInputSnapshot> updatedEntries{
            {.name = QStringLiteral("speed"),
             .path = QStringLiteral("speed"),
             .displayName = QStringLiteral("speed"),
             .kind = QStringLiteral("Number"),
             .source = QStringLiteral("ViewModel"),
             .value = 2.5},
            {.name = QStringLiteral("enabled"),
             .path = QStringLiteral("enabled"),
             .displayName = QStringLiteral("enabled"),
             .kind = QStringLiteral("Boolean"),
             .source = QStringLiteral("ViewModel"),
             .value = false},
        };

        model.setEntries(updatedEntries);

        QCOMPARE(resetSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 1);
        QCOMPARE(model.data(model.index(0, 0), RiveInputListModel::ValueRole).toDouble(), 2.5);
        QCOMPARE(model.data(model.index(1, 0), RiveInputListModel::ValueRole).toBool(), false);
    }

    void inputModelResetsWhenRowsChangeIdentity()
    {
        RiveInputListModel model;
        model.setEntries({
            {.name = QStringLiteral("speed"),
             .path = QStringLiteral("speed"),
             .displayName = QStringLiteral("speed"),
             .kind = QStringLiteral("Number"),
             .source = QStringLiteral("ViewModel"),
             .value = 1.0},
        });

        QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
        QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

        model.setEntries({
            {.name = QStringLiteral("mode"),
             .path = QStringLiteral("mode"),
             .displayName = QStringLiteral("mode"),
             .kind = QStringLiteral("String"),
             .source = QStringLiteral("Script"),
             .value = QStringLiteral("auto")},
        });

        QCOMPARE(resetSpy.count(), 1);
        QCOMPARE(dataChangedSpy.count(), 0);
    }

    void inputModelAppendsTailWithoutReset()
    {
        RiveInputListModel model;
        model.setEntries({
            {.name = QStringLiteral("speed"),
             .path = QStringLiteral("speed"),
             .displayName = QStringLiteral("speed"),
             .kind = QStringLiteral("Number"),
             .source = QStringLiteral("ViewModel"),
             .value = 1.0},
        });

        QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
        QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);

        model.setEntries({
            {.name = QStringLiteral("speed"),
             .path = QStringLiteral("speed"),
             .displayName = QStringLiteral("speed"),
             .kind = QStringLiteral("Number"),
             .source = QStringLiteral("ViewModel"),
             .value = 1.0},
            {.name = QStringLiteral("enabled"),
             .path = QStringLiteral("enabled"),
             .displayName = QStringLiteral("enabled"),
             .kind = QStringLiteral("Boolean"),
             .source = QStringLiteral("ViewModel"),
             .value = true},
        });

        QCOMPARE(resetSpy.count(), 0);
        QCOMPARE(rowsInsertedSpy.count(), 1);
        QCOMPARE(model.rowCount(), 2);
        QCOMPARE(model.data(model.index(1, 0), RiveInputListModel::NameRole).toString(),
                 QStringLiteral("enabled"));
    }

    void selectionModelTrimsTailWithoutReset()
    {
        RiveSelectionListModel model;
        model.setEntries({
            {.name = QStringLiteral("Main"), .current = true},
            {.name = QStringLiteral("HUD"), .current = false},
            {.name = QStringLiteral("Overlay"), .current = false},
        });

        QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
        QSignalSpy rowsRemovedSpy(&model, &QAbstractItemModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

        model.setEntries({
            {.name = QStringLiteral("Main"), .current = false},
            {.name = QStringLiteral("HUD"), .current = true},
        });

        QCOMPARE(resetSpy.count(), 0);
        QCOMPARE(rowsRemovedSpy.count(), 1);
        QCOMPARE(dataChangedSpy.count(), 1);
        QCOMPARE(model.rowCount(), 2);
        QCOMPARE(model.data(model.index(1, 0), RiveSelectionListModel::CurrentRole).toBool(),
                 true);
    }
};

QTEST_GUILESS_MAIN(SourceLoadingTest)

#include "test_source_loading.moc"
