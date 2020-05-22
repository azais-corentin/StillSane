#pragma once

#include <QNetworkRequest>
#include <QObject>

#include "types.hh"

namespace AutoTrade::Poe::Api {

struct CurrencyDetail {
  QString name, tradeId;
  int     ninjaId;
  double  chaosEquivalent = 1.0;
};

class Ninja : public QObject {
  Q_OBJECT
 public:
  explicit Ninja(QObject* parent = nullptr);

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

}  // namespace AutoTrade::Poe::Api
