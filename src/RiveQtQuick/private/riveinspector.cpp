#include "riveinspector.h"

#include <algorithm>

namespace {
template <typename Entry,
  typename IdentityMatcher,
  typename BeginRemoveRows,
  typename EndRemoveRows,
  typename BeginInsertRows,
  typename EndInsertRows,
  typename EmitDataChanged>
bool updateTrailingRows(QVector<Entry>& currentEntries,
  const QVector<Entry>& nextEntries,
  const QVector<int>& changedRoles,
  IdentityMatcher identityMatcher,
  BeginRemoveRows beginRemoveRows,
  EndRemoveRows endRemoveRows,
  BeginInsertRows beginInsertRows,
  EndInsertRows endInsertRows,
  EmitDataChanged emitDataChanged)
{
  if (currentEntries == nextEntries) {
    return true;
  }

  qsizetype sharedCount = std::min(currentEntries.size(), nextEntries.size());
  for (qsizetype i = 0; i < sharedCount; ++i) {
    if (!identityMatcher(currentEntries.at(i), nextEntries.at(i))) {
      return false;
    }
  }

  qsizetype oldCount = currentEntries.size();
  qsizetype newCount = nextEntries.size();

  if (newCount < oldCount) {
    beginRemoveRows(static_cast<int>(newCount), static_cast<int>(oldCount - 1));
    currentEntries.resize(newCount);
    endRemoveRows();
  } else if (newCount > oldCount) {
    beginInsertRows(static_cast<int>(oldCount), static_cast<int>(newCount - 1));
    currentEntries.resize(newCount);
    for (qsizetype i = oldCount; i < newCount; ++i) {
      currentEntries[i] = nextEntries.at(i);
    }
    endInsertRows();
  }

  int dirtyRangeStart = -1;
  int dirtyRangeEnd = -1;
  auto flushDirtyRange = [&]() {
    if (dirtyRangeStart < 0) {
      return;
    }

    emitDataChanged(dirtyRangeStart, dirtyRangeEnd, changedRoles);
    dirtyRangeStart = -1;
    dirtyRangeEnd = -1;
  };

  for (qsizetype i = 0; i < sharedCount; ++i) {
    if (currentEntries.at(i) == nextEntries.at(i)) {
      flushDirtyRange();
      continue;
    }

    if (dirtyRangeStart < 0) {
      dirtyRangeStart = static_cast<int>(i);
    }
    dirtyRangeEnd = static_cast<int>(i);
    currentEntries[static_cast<int>(i)] = nextEntries.at(i);
  }
  flushDirtyRange();

  return true;
}
} // namespace

bool operator==(const RiveSelectionEntrySnapshot& lhs,
  const RiveSelectionEntrySnapshot& rhs)
{
  return lhs.name == rhs.name && lhs.current == rhs.current;
}

bool operator==(const RiveInputSnapshot& lhs, const RiveInputSnapshot& rhs)
{
  return lhs.name == rhs.name && lhs.path == rhs.path && lhs.displayName == rhs.displayName && lhs.kind == rhs.kind && lhs.source == rhs.source && lhs.value == rhs.value && lhs.minimum == rhs.minimum && lhs.maximum == rhs.maximum;
}

bool operator==(const RiveEventSnapshot& lhs, const RiveEventSnapshot& rhs)
{
  return lhs.name == rhs.name && lhs.payload == rhs.payload && lhs.timestamp == rhs.timestamp && lhs.sourceStateMachine == rhs.sourceStateMachine;
}

RiveSelectionListModel::RiveSelectionListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int RiveSelectionListModel::rowCount(const QModelIndex& parent) const
{
  return parent.isValid() ? 0 : m_entries.size();
}

QVariant RiveSelectionListModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
    return {};
  }

  const RiveSelectionEntrySnapshot& entry = m_entries.at(index.row());
  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
    return entry.name;
  case CurrentRole:
    return entry.current;
  default:
    return {};
  }
}

QHash<int, QByteArray> RiveSelectionListModel::roleNames() const
{
  return {
    { NameRole, "name" },
    { CurrentRole, "current" },
  };
}

void RiveSelectionListModel::setEntries(
  const QVector<RiveSelectionEntrySnapshot>& entries)
{
  if (updateTrailingRows(
        m_entries,
        entries,
        { NameRole, CurrentRole, Qt::DisplayRole },
        [](const RiveSelectionEntrySnapshot& current,
          const RiveSelectionEntrySnapshot& next) {
          return current.name == next.name;
        },
        [this](int first, int last) {
          beginRemoveRows(QModelIndex(), first, last);
        },
        [this]() { endRemoveRows(); },
        [this](int first, int last) {
          beginInsertRows(QModelIndex(), first, last);
        },
        [this]() { endInsertRows(); },
        [this](int first, int last, const QVector<int>& roles) {
          emit dataChanged(index(first, 0),
            index(last, 0),
            roles);
        })) {
    return;
  }

  beginResetModel();
  m_entries = entries;
  endResetModel();
}

void RiveSelectionListModel::clear()
{
  setEntries({});
}

RiveInputListModel::RiveInputListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int RiveInputListModel::rowCount(const QModelIndex& parent) const
{
  return parent.isValid() ? 0 : m_entries.size();
}

QVariant RiveInputListModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
    return {};
  }

  const RiveInputSnapshot& entry = m_entries.at(index.row());
  switch (role) {
  case Qt::DisplayRole:
  case DisplayNameRole:
    return entry.displayName;
  case NameRole:
    return entry.name;
  case PathRole:
    return entry.path;
  case KindRole:
    return entry.kind;
  case SourceRole:
    return entry.source;
  case ValueRole:
    return entry.value;
  case MinimumRole:
    return entry.minimum;
  case MaximumRole:
    return entry.maximum;
  default:
    return {};
  }
}

QHash<int, QByteArray> RiveInputListModel::roleNames() const
{
  return {
    { NameRole, "name" },
    { PathRole, "path" },
    { DisplayNameRole, "displayName" },
    { KindRole, "kind" },
    { SourceRole, "source" },
    { ValueRole, "value" },
    { MinimumRole, "minimum" },
    { MaximumRole, "maximum" },
  };
}

void RiveInputListModel::setEntries(const QVector<RiveInputSnapshot>& entries)
{
  if (updateTrailingRows(
        m_entries,
        entries,
        { NameRole,
          PathRole,
          DisplayNameRole,
          KindRole,
          SourceRole,
          ValueRole,
          MinimumRole,
          MaximumRole,
          Qt::DisplayRole },
        [](const RiveInputSnapshot& current,
          const RiveInputSnapshot& next) {
          return current.name == next.name && current.path == next.path && current.displayName == next.displayName && current.kind == next.kind && current.source == next.source;
        },
        [this](int first, int last) {
          beginRemoveRows(QModelIndex(), first, last);
        },
        [this]() { endRemoveRows(); },
        [this](int first, int last) {
          beginInsertRows(QModelIndex(), first, last);
        },
        [this]() { endInsertRows(); },
        [this](int first, int last, const QVector<int>& roles) {
          emit dataChanged(index(first, 0),
            index(last, 0),
            roles);
        })) {
    return;
  }

  beginResetModel();
  m_entries = entries;
  endResetModel();
}

void RiveInputListModel::clear()
{
  setEntries({});
}

RiveEventListModel::RiveEventListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int RiveEventListModel::rowCount(const QModelIndex& parent) const
{
  return parent.isValid() ? 0 : m_entries.size();
}

QVariant RiveEventListModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
    return {};
  }

  const RiveEventSnapshot& entry = m_entries.at(index.row());
  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
    return entry.name;
  case PayloadRole:
    return entry.payload;
  case TimestampRole:
    return entry.timestamp;
  case SourceStateMachineRole:
    return entry.sourceStateMachine;
  default:
    return {};
  }
}

QHash<int, QByteArray> RiveEventListModel::roleNames() const
{
  return {
    { NameRole, "name" },
    { PayloadRole, "payload" },
    { TimestampRole, "timestamp" },
    { SourceStateMachineRole, "sourceStateMachine" },
  };
}

void RiveEventListModel::appendEntry(const RiveEventSnapshot& entry)
{
  bool dropFirst = m_entries.size() >= MaxEntries;
  if (dropFirst) {
    beginRemoveRows(QModelIndex(), 0, 0);
    m_entries.removeFirst();
    endRemoveRows();
  }

  const int row = m_entries.size();
  beginInsertRows(QModelIndex(), row, row);
  m_entries.append(entry);
  endInsertRows();
}

void RiveEventListModel::clear()
{
  beginResetModel();
  m_entries.clear();
  endResetModel();
}
