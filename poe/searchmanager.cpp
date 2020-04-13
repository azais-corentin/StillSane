#include "searchmanager.hh"

namespace AutoTrade::Poe {

SearchManager::SearchManager(QObject* parent) : QObject(parent) {}

void SearchManager::addSearch(const QString& id, const QString& name) {
  mSearches.append({id, name});

  emit searchAdded();
}

QVector<Search>& SearchManager::getSearches() {
  return mSearches;
}

}  // namespace AutoTrade::Poe
