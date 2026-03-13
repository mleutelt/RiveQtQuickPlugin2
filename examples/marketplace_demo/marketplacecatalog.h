#pragma once

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QVector>
#include <QUrl>
#include <QVariantMap>

class QNetworkReply;

struct MarketplaceCatalogEntry
{
    QString id;
    QString title;
    QString detailUrl;
    QString author;
    bool isForHire { false };
    QString license;
    QStringList tags;
    QString description;
    QString previewImageUrl;
    QString riveSource;
    QString cachedRiveSource;
    QString availability;
    bool isDownloading { false };
    QString downloadError;
    qint64 downloadBytes { 0 };
    qint64 downloadTotalBytes { 0 };
    int reactionCount { 0 };
};

class MarketplaceCatalogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged FINAL)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged FINAL)
    Q_PROPERTY(QVariantMap currentMetadata READ currentMetadata NOTIFY currentMetadataChanged FINAL)
    Q_PROPERTY(QUrl currentRiveSource READ currentRiveSource NOTIFY currentRiveSourceChanged FINAL)
    Q_PROPERTY(bool currentHasPreview READ currentHasPreview NOTIFY currentHasPreviewChanged FINAL)
    Q_PROPERTY(bool isLoadingMore READ isLoadingMore NOTIFY isLoadingMoreChanged FINAL)
    Q_PROPERTY(bool hasMoreRemoteEntries READ hasMoreRemoteEntries NOTIFY hasMoreRemoteEntriesChanged FINAL)

public:
    enum Role
    {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        DetailUrlRole,
        AuthorRole,
        IsForHireRole,
        LicenseRole,
        TagsRole,
        DescriptionRole,
        PreviewImageUrlRole,
        RiveSourceRole,
        AvailabilityRole,
        ReactionCountRole,
        CurrentRole,
        HasPreviewRole,
    };

    explicit MarketplaceCatalogModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString searchText() const;
    void setSearchText(const QString& searchText);

    int currentIndex() const;
    void setCurrentIndex(int currentIndex);

    QVariantMap currentMetadata() const;
    QUrl currentRiveSource() const;
    bool currentHasPreview() const;
    bool isLoadingMore() const;
    bool hasMoreRemoteEntries() const;

    void initialize(const QString& sourceDir);
    bool loadFromFile(const QString& catalogPath, const QString& sourceDir);
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE void downloadCurrent();

signals:
    void searchTextChanged();
    void currentIndexChanged();
    void currentMetadataChanged();
    void currentRiveSourceChanged();
    void currentHasPreviewChanged();
    void isLoadingMoreChanged();
    void hasMoreRemoteEntriesChanged();

private:
    void appendEntries(const QVector<MarketplaceCatalogEntry>& entries);
    void setLoadingMore(bool loading);
    void setHasMoreRemoteEntries(bool hasMore);
    int indexOfEntryId(const QString& id) const;
    int visibleRowForEntryIndex(int entryIndex) const;
    int currentEntryIndex() const;
    void notifyEntryUpdated(int entryIndex, const QVector<int>& roles);
    void rebuildVisibleEntries();
    bool matchesFilter(const MarketplaceCatalogEntry& entry) const;
    void setCurrentEntryById(const QString& id);
    void hydrateCachedEntry(MarketplaceCatalogEntry* entry) const;
    QString cacheFilePathForEntry(const MarketplaceCatalogEntry& entry) const;
    const MarketplaceCatalogEntry* entryAtVisibleIndex(int index) const;
    QVariantMap metadataForEntry(const MarketplaceCatalogEntry& entry) const;
    QUrl sourceUrlForEntry(const MarketplaceCatalogEntry& entry) const;
    static MarketplaceCatalogEntry entryFromJson(const QJsonObject& object);
    static MarketplaceCatalogEntry entryFromApiJson(const QJsonObject& object);

    QVector<MarketplaceCatalogEntry> m_entries;
    QVector<int> m_visibleIndexes;
    QString m_searchText;
    QString m_sourceDir;
    QString m_currentId;
    int m_currentIndex { -1 };
    int m_nextFeaturedPage { 0 };
    bool m_isLoadingMore { false };
    bool m_hasMoreRemoteEntries { true };
    QNetworkAccessManager m_networkAccessManager;
    QPointer<QNetworkReply> m_loadReply;
    QPointer<QNetworkReply> m_downloadReply;
};
