#include "searchresulttree.hh"

namespace AutoTrade::Ui::Models {

SearchResultTree::SearchResultTree(QObject* parent) : QAbstractItemModel(parent) {}

QVariant SearchResultTree::headerData(int             section,
                                      Qt::Orientation orientation,
                                      int             role) const {
  // FIXME: Implement me!
  return {};
}

QModelIndex SearchResultTree::index(int                row,
                                    int                column,
                                    const QModelIndex& parent) const {
  // FIXME: Implement me!
  return {};
}

QModelIndex SearchResultTree::parent(const QModelIndex& index) const {
  // FIXME: Implement me!
  return {};
}

int SearchResultTree::rowCount(const QModelIndex& parent) const {
  if (!parent.isValid())
    return 0;

  // FIXME: Implement me!
  return {};
}

int SearchResultTree::columnCount(const QModelIndex& parent) const {
  if (!parent.isValid())
    return 0;

  // FIXME: Implement me!
}

QVariant SearchResultTree::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  // FIXME: Implement me!
  return QVariant();
}

}  // namespace AutoTrade::Ui::Models
