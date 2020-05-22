#pragma once

#include <memory>

#include <QAbstractTableModel>

#include "../../poe/searchmanager.hh"

namespace StillSane::Poe::Api {
class Trade;
}

namespace StillSane::Ui::Models {

class SearchTable : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit SearchTable(QObject* parent = nullptr);

  void setSearches(const QVector<Poe::Api::Trade*>& tradeApis);

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
  void nameChanged(QString id, const QString& name);
  void enableChanged(QString id, bool enabled);

 private:
  void onSearchAdded();

 private:
  QVector<Poe::Api::Trade*> mTradeApis;
};

}  // namespace StillSane::Ui::Models
