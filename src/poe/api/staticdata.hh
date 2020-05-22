#pragma once

#include <QNetworkRequest>
#include <QObject>

#include <poe/api/types.hh>

namespace AutoTrade::Poe::Api {

class StaticData : public QObject {
  Q_OBJECT
 public:
  explicit StaticData(QObject* parent = nullptr);
  ~StaticData();

  void fetchItems();
  void fetchStats();
  void fetchStatic();

 signals:
  void itemsFetched(const QVector<Item>& items);
  void statsFetched();
  void staticFetched();

 private:
  void parseFetchedItems(const QByteArray& data);
  void parseFetchedStats(const QByteArray& data);
  void parseFetchedStatic(const QByteArray& data);

  QNetworkRequest buildRequest(const QString& path) const;

  QVector<Item> mItems;
};

}  // namespace AutoTrade::Poe::Api
