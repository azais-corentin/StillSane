#include "searchmanager.hh"

#include "api/constants.hh"
#include "api/trade.hh"

namespace StillSane::Poe {

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
                              const QString& name,
                              bool           enabled) {
  // Create a new live Trade
  auto* trade = new Api::Trade{};
  trade->setId(id);
  trade->setName(name);
  trade->setLeague(league);
  trade->setEnabled(enabled);

  // Add it to the vector
  mTradeApis.push_back(trade);

  // Connect signals and start the live search
  connectToTradeApi(trade);

  if (enabled) {
    trade->openLiveSearch();
  }

  emit searchAdded();
}

auto SearchManager::editSearch(const QString& id, const QString& name) -> bool {
  auto it = std::find_if(mTradeApis.begin(), mTradeApis.end(),
                         [&id](Api::Trade* trade) { return trade->getId() == id; });

  if (it != mTradeApis.end()) {
    (*it)->setName(name);
    return true;
  }

  return false;
}

auto SearchManager::editSearch(const QString& id, const bool& enabled) -> bool {
  auto it = std::find_if(mTradeApis.begin(), mTradeApis.end(),
                         [&id](Api::Trade* trade) { return trade->getId() == id; });

  if (it != mTradeApis.end()) {
    (*it)->setEnabled(enabled);
    return true;
  }

  return false;
}

auto SearchManager::getSearches() -> QVector<Api::Trade*>& {
  return mTradeApis;
}

void SearchManager::connectToTradeApi(Api::Trade* trade) {
  connect(trade, &Api::Trade::itemReceived,
          [&](const Api::TradeItem&) { /*onItemReceived(trade, item);*/ });
}

void SearchManager::onItemReceived(size_t index, const Api::TradeItem& item) {
  qDebug() << "trade[" << index << "] result:" << item.text;
}

}  // namespace StillSane::Poe
