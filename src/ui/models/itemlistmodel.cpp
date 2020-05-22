#include "itemlistmodel.hh"

#include <QDebug>
#include <QPair>

namespace AutoTrade::ui::models {

ItemList::ItemList(const QVector<Poe::Api::Item>& items, QObject* parent)
    : QAbstractListModel(parent), mItems(items) {
  qDebug() << "Created model with" << mItems.size() << "items!";
  qDebug() << "Sorting items!";

  std::sort(mItems.begin(), mItems.end(), [](const auto& a, const auto& b) {
    return QString::localeAwareCompare(a.text, b.text) <= 0;
  });
  qDebug() << "Done sorting.";
}

int ItemList::rowCount(const QModelIndex& parent) const {
  return mItems.size();
}

QVariant ItemList::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() >= rowCount())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    return mItems.at(index.row()).text;
  } else if (role == Qt::UserRole) {
    auto item = mItems.at(index.row());
    return QVariant::fromValue(std::make_pair(item.name, item.type));
  } else
    return QVariant();
}

QVariant ItemList::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
    return QStringLiteral("Item name");
  else
    return QStringLiteral("Row %1").arg(section);
}

}  // namespace AutoTrade::ui::models
