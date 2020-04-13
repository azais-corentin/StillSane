#ifndef POE_NINJAAPI_HH
#define POE_NINJAAPI_HH

#include <QNetworkRequest>
#include <QObject>

#include <poe/types.hh>

namespace AutoTrade::Poe {

struct CurrencyDetail {
  QString name, tradeId;
  int     ninjaId;
  double  chaosEquivalent = 1.0;
};

class NinjaAPI : public QObject {
  Q_OBJECT
 public:
  explicit NinjaAPI(QObject* parent = nullptr);

  double getChaosEquivalent(const QString& tradeId);

  void fetchCurrencyOverview(const QString& league, const QString& currencyType);
  void fetchCurrencyHistory(const QString& league,
                            const QString& currencyType,
                            unsigned int   currencyId);
  void fetchItemOverview(const QString& league, const QString& itemType);
  void fetchItemHistory(const QString& league,
                        const QString& itemType,
                        unsigned int   itemid);

 private:
  void parseCurrencyOverview(const QByteArray& data);
  void parseCurrencyHistory(const QByteArray& data);
  void parseItemOverview(const QByteArray& data);
  void parseItemHistory(const QByteArray& data);

  QNetworkRequest buildRequest(const QString& path) const;

  QVector<CurrencyDetail> mCurrencyNames;

  bool mUpToDateCurrencyDetail = false;
  bool mUpdatingCurrencyDetail = false;
};

}  // namespace AutoTrade::Poe

#endif  // POE_NINJAAPI_HH
