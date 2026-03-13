#pragma once

#include <QAbstractTableModel>
#include <QUrl>

class IconGridModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(int artboardCount READ artboardCount NOTIFY artboardsChanged FINAL)

public:
    enum Role
    {
        ArtboardNumberRole = Qt::UserRole + 1,
    };

    explicit IconGridModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QUrl source() const;
    void setSource(const QUrl& source);

    bool isReady() const;
    QString errorString() const;
    int artboardCount() const;
    Q_INVOKABLE int artboardNumberAt(int row, int column) const;

signals:
    void sourceChanged();
    void readyChanged();
    void errorStringChanged();
    void artboardsChanged();

private:
    void loadArtboards();
    QString localFilePath() const;
    void setArtboards(const QStringList& artboards);
    void setErrorString(const QString& errorString);

    static constexpr int RowCountValue = 100;
    static constexpr int ColumnCountValue = 100;

    QUrl m_source;
    QStringList m_artboards;
    QString m_errorString;
};
