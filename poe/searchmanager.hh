#ifndef AUTOTRADE_POE_SEARCHMANAGER_HH
#define AUTOTRADE_POE_SEARCHMANAGER_HH

#include <QObject>

namespace AutoTrade::Poe {

struct Search {
  QString id, name;
  bool    enabled = true;
};

class SearchManager : public QObject {
  Q_OBJECT

 public:
  explicit SearchManager(QObject* parent = nullptr);

  void addSearch(const QString& id, const QString& name);

  QVector<Search>& getSearches();

 signals:
  void searchAdded();
  void newItemsReceived();

 private:
  QVector<Search> mSearches;
};

}  // namespace AutoTrade::Poe

#endif  // AUTOTRADE_POE_SEARCHMANAGER_HH
