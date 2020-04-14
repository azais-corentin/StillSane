#ifndef AUTOTRADE_POE_SEARCHMANAGER_HH
#define AUTOTRADE_POE_SEARCHMANAGER_HH

#include <memory>

#include <QObject>
#include <QVector>

//#include <poe/api/trade.hh>
#include <poe/api/types.hh>

namespace AutoTrade::Poe {

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
  void newItemsReceived();

 private:
  void connectToTradeApi(Api::Trade* trade);

  void onItemReceived(size_t index, const Api::TradeItem& item);

 private:
  QVector<Api::Trade*> mTradeApis;

  QString mPOESESSID;
};

}  // namespace AutoTrade::Poe

#endif  // AUTOTRADE_POE_SEARCHMANAGER_HH
