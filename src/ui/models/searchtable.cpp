#include "searchtable.hh"

#include <QLabel>

#include "../../poe/api/trade.hh"
#include "../../poe/searchmanager.hh"

namespace StillSane::Ui::Models {

SearchTable::SearchTable(QObject* parent) : QAbstractTableModel(parent) {}

void SearchTable::setSearches(const QVector<Poe::Api::Trade*>& tradeApis) {
  beginResetModel();
  mTradeApis = tradeApis;
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

  return mTradeApis.size();
}

int SearchTable::columnCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return 3;
}

bool SearchTable::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (role != Qt::EditRole)
    return false;

  QString id = mTradeApis.at(index.row())->getId();

  switch (index.column()) {
    case 0: {
      QString name = value.toString();
      if (!name.isEmpty())
        mTradeApis.at(index.row())->setName(name);
      nameChanged(id, name);
      break;
    }
    case 2:
      mTradeApis.at(index.row())->setEnabled(value.toBool());
      enableChanged(id, value.toBool());
      break;
    default:
      return false;
  }

  emit dataChanged(index, index, {role});

  return true;
}

QVariant SearchTable::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return {};

  if (index.row() >= mTradeApis.size())
    return {};

  const auto& tradeApi = mTradeApis.at(index.row());

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0:
        return tradeApi->getName();
      case 1:
        return tradeApi->getId();
      case 2:
        return tradeApi->isEnabled() ? QStringLiteral("Enabled")
                                     : QStringLiteral("Disabled");
    }
  } else if (role == Qt::EditRole) {
    switch (index.column()) {
      case 0:
        return tradeApi->getName();
      case 2:
        return tradeApi->isEnabled();
    }
  } else if (role == Qt::TextAlignmentRole) {
    switch (index.column()) {
      case 0:
        [[fallthrough]];
      case 1:
        return Qt::AlignVCenter + Qt::AlignLeft;
      case 2:
        return Qt::AlignVCenter + Qt::AlignHCenter;
    }
  } else if (role == Qt::CheckStateRole) {
    switch (index.column()) {
      case 2:
        return Qt::CheckState{tradeApi->isEnabled() ? Qt::Checked : Qt::Unchecked};
    }
  }

  return {};
}

Qt::ItemFlags SearchTable::flags(const QModelIndex& index) const {
  return QAbstractTableModel::flags(index) |
         (index.column() != 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

}  // namespace StillSane::Ui::Models
