#include "searchmanager.hh"

#include <poe/api/trade.hh>

namespace AutoTrade::Poe {

SearchManager::SearchManager(QObject* parent) : QObject(parent) {}

void SearchManager::setCookies(const QString& POESESSID) {
  mPOESESSID = POESESSID;

  Network::AccessManager::setPOESSID(POESESSID);
}

void SearchManager::addSearch(const QString& id, const QString& name) {
  // Add search info
  mSearches.append({id, name});

  // Add and start live search
  mTradeApis.emplace_back(std::make_unique<Api::Trade>());
  mTradeApis.back()->openLiveSearch(id);

  emit searchAdded();
}

bool SearchManager::editSearch(const QString& id, const QString& name) {
  auto it = std::find_if(mSearches.begin(), mSearches.end(),
                         [&id](const Search& search) { return search.id == id; });

  if (it != mSearches.end()) {
    it->name = name;
    return true;
  }

  return false;
}

bool SearchManager::editSearch(const QString& id, const bool& enabled) {
  auto it = std::find_if(mSearches.begin(), mSearches.end(),
                         [&id](const Search& search) { return search.id == id; });

  if (it != mSearches.end()) {
    it->enabled = enabled;
    return true;
  }

  return false;
}

QVector<Search>& SearchManager::getSearches() {
  return mSearches;
}

}  // namespace AutoTrade::Poe
