#include "searchmanager.hh"

#include <poe/api/constants.hh>
#include <poe/api/trade.hh>

namespace AutoTrade::Poe {

SearchManager::SearchManager(QObject* parent) : QObject(parent) {}

SearchManager::~SearchManager() {
  qDeleteAll(mTradeApis);
}

void SearchManager::setCookies(const QString& POESESSID) {
  mPOESESSID = POESESSID;

  Network::AccessManager::setPOESESSID(POESESSID);
}

void SearchManager::addSearch(const QString& id,
                              const QString& league,
                              const QString& name) {
  // Create a new live Trade
  Api::Trade* trade = new Api::Trade{};
  trade->setId(id);
  trade->setName(name);
  trade->setLeague(league);

  // Add it to the vector
  mTradeApis.push_back(trade);

  // Connect signals and start the live search
  connectToTradeApi(trade);
  trade->openLiveSearch();

  emit searchAdded();
}

bool SearchManager::editSearch(const QString& id, const QString& name) {
  auto it = std::find_if(mTradeApis.begin(), mTradeApis.end(),
                         [&id](Api::Trade* trade) { return trade->getId() == id; });

  if (it != mTradeApis.end()) {
    (*it)->setName(name);
    return true;
  }

  return false;
}

bool SearchManager::editSearch(const QString& id, const bool& enabled) {
  auto it = std::find_if(mTradeApis.begin(), mTradeApis.end(),
                         [&id](Api::Trade* trade) { return trade->getId() == id; });

  if (it != mTradeApis.end()) {
    (*it)->setEnabled(enabled);
    return true;
  }

  return false;
}

QVector<Api::Trade*>& SearchManager::getSearches() {
  return mTradeApis;
}

void SearchManager::connectToTradeApi(Api::Trade* trade) {
  connect(trade, &Api::Trade::itemReceived,
          [&](const Api::TradeItem&) { /*onItemReceived(trade, item);*/ });
}

void SearchManager::onItemReceived(size_t index, const Api::TradeItem& item) {
  qDebug() << "trade[" << index << "] result:" << item.text;
}

}  // namespace AutoTrade::Poe
