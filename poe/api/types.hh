#ifndef AUTOTRADE_POE_API_TYPES_HH
#define AUTOTRADE_POE_API_TYPES_HH

#include <QString>

namespace AutoTrade::Poe::Api {

enum class League { Delirium, HardcoreDelirium, Standard, Hardcore };
struct League_t {
  const QString toString(League l) const {
    switch (l) {
      case League::Delirium:
        return QStringLiteral("Delirium");
      case League::HardcoreDelirium:
        return QStringLiteral("Hardcore Delirium");
      case League::Standard:
        return QStringLiteral("Standard");
      case League::Hardcore:
        return QStringLiteral("Hardcore");
    }
  }
};

struct Query {
  QString name, type;
};

struct Item {
  QString text, name, type;
};

struct TradeItem {
  QString text, name, type;

  int     price;
  QString currencyTradeId;
};

}  // namespace AutoTrade::Poe::Api

#endif  // AUTOTRADE_POE_API_TYPES_HH
