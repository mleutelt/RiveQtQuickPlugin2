#pragma once

#include <QAbstractListModel>
#include <QMetaType>
#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QVector>

struct RiveSelectionEntrySnapshot {
  QString name;
  bool current { false };
};

Q_DECLARE_METATYPE(RiveSelectionEntrySnapshot)
Q_DECLARE_METATYPE(QVector<RiveSelectionEntrySnapshot>)

bool operator==(const RiveSelectionEntrySnapshot& lhs,
  const RiveSelectionEntrySnapshot& rhs);

struct RiveInputSnapshot {
  QString name;
  QString path;
  QString displayName;
  QString kind;
  QString source;
  QVariant value;
  QVariant minimum;
  QVariant maximum;
};

Q_DECLARE_METATYPE(RiveInputSnapshot)
Q_DECLARE_METATYPE(QVector<RiveInputSnapshot>)

bool operator==(const RiveInputSnapshot& lhs, const RiveInputSnapshot& rhs);

struct RiveEventSnapshot {
  QString name;
  QVariantMap payload;
  qint64 timestamp { 0 };
  QString sourceStateMachine;
};

Q_DECLARE_METATYPE(RiveEventSnapshot)

bool operator==(const RiveEventSnapshot& lhs, const RiveEventSnapshot& rhs);

class RiveSelectionListModel : public QAbstractListModel {
  Q_OBJECT

  public:
  enum Role {
    NameRole = Qt::UserRole + 1,
    CurrentRole,
  };

  explicit RiveSelectionListModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
    int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void setEntries(const QVector<RiveSelectionEntrySnapshot>& entries);
  void clear();

  private:
  QVector<RiveSelectionEntrySnapshot> m_entries;
};

class RiveInputListModel : public QAbstractListModel {
  Q_OBJECT

  public:
  enum Role {
    NameRole = Qt::UserRole + 1,
    PathRole,
    DisplayNameRole,
    KindRole,
    SourceRole,
    ValueRole,
    MinimumRole,
    MaximumRole,
  };

  explicit RiveInputListModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
    int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void setEntries(const QVector<RiveInputSnapshot>& entries);
  void clear();

  private:
  QVector<RiveInputSnapshot> m_entries;
};

class RiveEventListModel : public QAbstractListModel {
  Q_OBJECT

  public:
  enum Role {
    NameRole = Qt::UserRole + 1,
    PayloadRole,
    TimestampRole,
    SourceStateMachineRole,
  };

  explicit RiveEventListModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
    int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void appendEntry(const RiveEventSnapshot& entry);
  void clear();

  private:
  QVector<RiveEventSnapshot> m_entries;
  static constexpr int MaxEntries = 128;
};
