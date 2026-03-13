#include "icongridmodel.h"

#include <QFile>

#include "rive/file.hpp"
#include "utils/no_op_factory.hpp"

namespace
{
int seededArtboardIndex(int row, int column, int artboardCount)
{
    if (artboardCount <= 1)
    {
        return 0;
    }

    quint32 seed = static_cast<quint32>((row + 1) * 1315423911u) ^
                   static_cast<quint32>((column + 7) * 2654435761u);
    return 1 + static_cast<int>(seed % static_cast<quint32>(artboardCount - 1));
}
}

IconGridModel::IconGridModel(QObject* parent) : QAbstractTableModel(parent) {}

int IconGridModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : RowCountValue;
}

int IconGridModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : ColumnCountValue;
}

QVariant IconGridModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::DisplayRole || role == ArtboardNumberRole)
    {
        return artboardNumberAt(index.row(), index.column());
    }

    return {};
}

QHash<int, QByteArray> IconGridModel::roleNames() const
{
    return {
        {ArtboardNumberRole, "artboardNumber"},
    };
}

QUrl IconGridModel::source() const
{
    return m_source;
}

void IconGridModel::setSource(const QUrl& source)
{
    if (m_source == source)
    {
        return;
    }

    m_source = source;
    emit sourceChanged();
    loadArtboards();
}

bool IconGridModel::isReady() const
{
    return !m_artboards.isEmpty();
}

QString IconGridModel::errorString() const
{
    return m_errorString;
}

int IconGridModel::artboardCount() const
{
    return m_artboards.size();
}

int IconGridModel::artboardNumberAt(int row, int column) const
{
    if (m_artboards.isEmpty())
    {
        return 0;
    }

    return seededArtboardIndex(row, column, m_artboards.size());
}

void IconGridModel::loadArtboards()
{
    QStringList artboards;
    QString errorString;
    QString filePath;

    if (!m_source.isValid() || m_source.isEmpty())
    {
        errorString = "The icon grid example needs a local .riv file.";
    }
    else if (m_source.isLocalFile())
    {
        filePath = m_source.toLocalFile();
    }
    else if (m_source.scheme() == "qrc")
    {
        filePath = ":" + m_source.path();
    }
    else if (m_source.scheme().isEmpty())
    {
        filePath = m_source.toString();
    }
    else
    {
        errorString = "Only local .riv files are supported.";
    }

    if (errorString.isEmpty())
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            errorString = file.errorString();
        }
        else
        {
            QByteArray bytes = file.readAll();
            rive::ImportResult importResult = rive::ImportResult::success;
            static rive::NoOpFactory factory;
            auto riveFile = rive::File::import(
                rive::Span<const uint8_t>(
                    reinterpret_cast<const uint8_t*>(bytes.constData()),
                    static_cast<size_t>(bytes.size())),
                &factory,
                &importResult);

            if (!riveFile || importResult != rive::ImportResult::success)
            {
                errorString = "Failed to read the icon set .riv file.";
            }
            else
            {
                artboards.reserve(static_cast<int>(riveFile->artboardCount()));
                for (size_t i = 0; i < riveFile->artboardCount(); ++i)
                {
                    artboards.append(QString::fromStdString(riveFile->artboardNameAt(i)));
                }
            }
        }
    }

    setArtboards(artboards);
    setErrorString(errorString);
}

void IconGridModel::setArtboards(const QStringList& artboards)
{
    if (m_artboards == artboards)
    {
        return;
    }

    const bool readyChangedNow = m_artboards.isEmpty() != artboards.isEmpty();
    beginResetModel();
    m_artboards = artboards;
    endResetModel();

    emit artboardsChanged();
    if (readyChangedNow)
    {
        emit readyChanged();
    }
}

void IconGridModel::setErrorString(const QString& errorString)
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}
