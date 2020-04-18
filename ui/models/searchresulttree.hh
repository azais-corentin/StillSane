#ifndef AUTOTRADE_UI_MODELS_SEARCHRESULTTREE_HH
#define AUTOTRADE_UI_MODELS_SEARCHRESULTTREE_HH

#include <QAbstractItemModel>

namespace AutoTrade::Ui::Models {

class SearchResultTree : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit SearchResultTree(QObject* parent = nullptr);

  // Header:
  QVariant headerData(int             section,
                      Qt::Orientation orientation,
                      int             role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int                row,
                    int                column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

 private:
};

}  // namespace AutoTrade::Ui::Models

#endif  // AUTOTRADE_UI_MODELS_SEARCHRESULTTREE_HH
