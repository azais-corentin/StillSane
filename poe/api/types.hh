#ifndef AUTOTRADE_POE_API_TYPES_HH
#define AUTOTRADE_POE_API_TYPES_HH

#include <QMap>
#include <QString>

namespace AutoTrade::Poe::Api {

enum Currency { Mirror, Exalted, Chaos };

const QMap<Currency, QString> CurrencyName{{Mirror, "mirror"},
                                           {Exalted, "exalted"},
                                           {Chaos, "chaos"}};

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
