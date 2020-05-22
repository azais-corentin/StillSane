#pragma once

#include <QMap>
#include <QString>

namespace StillSane::Poe::Api {

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

}  // namespace StillSane::Poe::Api
