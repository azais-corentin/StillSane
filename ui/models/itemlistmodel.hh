#ifndef UI_MODELS_ITEMLISTMODEL_HH
#define UI_MODELS_ITEMLISTMODEL_HH

#include <QAbstractListModel>

#include <poe/api/types.hh>

namespace AutoTrade::ui::models {

class ItemList : public QAbstractListModel {
  Q_OBJECT
 public:
  ItemList(const QVector<Poe::Item>& items, QObject* parent = nullptr);

  int      rowCount(const QModelIndex& parent = QModelIndex()) const final;
  QVariant data(const QModelIndex& index, int role) const final;
  QVariant headerData(int             section,
                      Qt::Orientation orientation,
                      int             role = Qt::DisplayRole) const final;

 private:
  QVector<Poe::Item> mItems;
};

}  // namespace AutoTrade::ui::models

#endif  // UI_MODELS_ITEMLISTMODEL_HH
