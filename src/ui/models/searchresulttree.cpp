#include "searchresulttree.hh"

namespace StillSane::Ui::Models {

SearchResultTree::SearchResultTree(QObject* parent) : QAbstractItemModel(parent) {}

auto SearchResultTree::headerData(int /*section*/,
                                  Qt::Orientation /*orientation*/,
                                  int /*role*/) const -> QVariant {
  //  FIXME: Implement me!
  return {};
}

auto SearchResultTree::index(int /*row*/,
                             int /*column*/,
                             const QModelIndex& /*parent*/) const -> QModelIndex {
  // FIXME: Implement me!
  return {};
}

auto SearchResultTree::parent(const QModelIndex& /*index*/) const -> QModelIndex {
  // FIXME: Implement me!
  return {};
}

auto SearchResultTree::rowCount(const QModelIndex& parent) const -> int {
  if (!parent.isValid()) {
    return 0;
  }

  // FIXME: Implement me!
  return {};
}

auto SearchResultTree::columnCount(const QModelIndex& parent) const -> int {
  if (!parent.isValid()) {
    return 0;
  }

  // FIXME: Implement me!
  return 0;
}

auto SearchResultTree::data(const QModelIndex& index, int /*role*/) const -> QVariant {
  if (!index.isValid()) {
    return {};
  }

  // FIXME: Implement me!
  return QVariant();
}

}  // namespace StillSane::Ui::Models
