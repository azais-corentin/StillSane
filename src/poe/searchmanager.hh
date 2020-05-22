#pragma once

#include <memory>

#include <QObject>
#include <QVector>

#include "api/types.hh"

namespace StillSane::Poe {

struct Result {};

namespace Api {
class Trade;
}

class SearchManager : public QObject {
  Q_OBJECT

 public:
  explicit SearchManager(QObject* parent = nullptr);
  ~SearchManager();

  void setCookies(const QString& POESESSID);

  void addSearch(const QString& id,
                 const QString& league,
                 const QString& name,
                 bool           enabled = true);
  bool editSearch(const QString& id, const QString& name);
  bool editSearch(const QString& id, const bool& enabled);

  QVector<Api::Trade*>& getSearches();

 signals:
  void searchAdded();
  void newResult();

 private:
  void connectToTradeApi(Api::Trade* trade);

  void onItemReceived(size_t index, const Api::TradeItem& item);

 private:
  QVector<Api::Trade*> mTradeApis;

  QString mPOESESSID;
};

}  // namespace StillSane::Poe
