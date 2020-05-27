#pragma once

#include <QNetworkRequest>
#include <QObject>

#include "types.hh"

namespace StillSane::Poe::Api {

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

  static QNetworkRequest buildRequest(const QString& path) ;

  QVector<Item> mItems;
};

}  // namespace StillSane::Poe::Api
