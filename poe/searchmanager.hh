#ifndef AUTOTRADE_POE_SEARCHMANAGER_HH
#define AUTOTRADE_POE_SEARCHMANAGER_HH

#include <memory>
#include <vector>

#include <QObject>
#include <QVector>

#include <poe/api/trade.hh>

namespace AutoTrade::Poe {

struct Search {
  QString id, name;
  bool    enabled = true;
};

class SearchManager : public QObject {
  Q_OBJECT

 public:
  explicit SearchManager(QObject* parent = nullptr);

  void setCookies(const QString& POESESSID);

  void addSearch(const QString& id, const QString& name);
  bool editSearch(const QString& id, const QString& name);
  bool editSearch(const QString& id, const bool& enabled);

  QVector<Search>& getSearches();

 signals:
  void searchAdded();
  void newItemsReceived();

 private:
  QVector<Search>                          mSearches;
  std::vector<std::unique_ptr<Api::Trade>> mTradeApis;

  QString mPOESESSID;
};

}  // namespace AutoTrade::Poe

#endif  // AUTOTRADE_POE_SEARCHMANAGER_HH
