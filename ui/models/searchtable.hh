#ifndef AUTOTRADE_UI_MODELS_SEARCHTABLE_HH
#define AUTOTRADE_UI_MODELS_SEARCHTABLE_HH

#include <memory>

#include <QAbstractTableModel>

#include <poe/searchmanager.hh>

namespace AutoTrade::Ui::Models {

class SearchTable : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit SearchTable(QObject* parent = nullptr);

  void setSearches(const QVector<Poe::Search>& searches);

  // Header:
  QVariant headerData(int             section,
                      Qt::Orientation orientation,
                      int             role = Qt::DisplayRole) const override;

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  bool     setData(const QModelIndex& index, const QVariant& value, int role) override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

 signals:
  void searchChanged(int row, const Poe::Search& data);

 private:
  void onSearchAdded();

 private:
  QVector<Poe::Search> mSearches;
};

}  // namespace AutoTrade::Ui::Models

#endif  // AUTOTRADE_UI_MODELS_SEARCHTABLE_HH
