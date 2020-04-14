#include "searchtable.hh"

#include <poe/searchmanager.hh>

#include <QLabel>

namespace AutoTrade::Ui::Models {

SearchTable::SearchTable(QObject* parent) : QAbstractTableModel(parent) {}

void SearchTable::setSearches(const QVector<Poe::Search>& searches) {
  beginResetModel();
  mSearches = searches;
  endResetModel();
}

QVariant SearchTable::headerData(int             section,
                                 Qt::Orientation orientation,
                                 int             role) const {
  if (role != Qt::DisplayRole)
    return {};

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return tr("Search name");
      case 1:
        return tr("Id");
      case 2:
        return tr("Enabled");
    }
  } else {
    return QString::number(section + 1);
  }

  return {};
}

int SearchTable::rowCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return mSearches.size();
}

int SearchTable::columnCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return 3;
}

bool SearchTable::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (role != Qt::EditRole)
    return false;

  switch (index.column()) {
    case 0:
      mSearches[index.row()].name = value.toString();
      break;
    case 2:
      mSearches[index.row()].enabled = value.toBool();
      break;
    default:
      return false;
  }

  emit dataChanged(index, index, {role});
  emit searchChanged(index.row(), mSearches.at(index.row()));

  return true;
}

QVariant SearchTable::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return {};

  if (index.row() >= mSearches.size())
    return {};

  auto&& search = mSearches.value(index.row());

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return search.name;
      case 1:
        return search.id;
      case 2:
        return search.enabled ? QStringLiteral("✔") : QStringLiteral("✖");
    }
  } else if (role == Qt::EditRole) {
    switch (index.column()) {
      case 0:
        return search.name;
      case 2:
        return search.enabled;
    }
  } else if (role == Qt::TextAlignmentRole) {
    switch (index.column()) {
      case 0:
      case 1:
        return Qt::AlignLeft;
      case 2:
        return Qt::AlignHCenter;
    }
  }

  return {};
}

Qt::ItemFlags SearchTable::flags(const QModelIndex& index) const {
  return QAbstractTableModel::flags(index) |
         (index.column() != 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

}  // namespace AutoTrade::Ui::Models
