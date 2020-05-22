#pragma once

#include <QString>

namespace StillSane::Poe::Api {

inline const QString currentLeague = QStringLiteral("Delirium");

/// Official API
inline const QString baseHostOfficial = QStringLiteral("www.pathofexile.com");
inline const QString basePathOfficial = QStringLiteral("/api/trade");
// Static
inline const QString StaticBasePath   = QStringLiteral("/data");
inline const QString StaticItemsPath  = QStringLiteral("/items");
inline const QString StaticStatsPath  = QStringLiteral("/stats");
inline const QString StaticStaticPath = QStringLiteral("/static");
// Trade
inline const QString TradeSearchPath = QStringLiteral("/search/");
inline const QString TradeFetchPath  = QStringLiteral("/fetch/");

// poe.ninja API
inline const QString baseHostNinja = QStringLiteral("poe.ninja");
inline const QString basePathNinja = QStringLiteral("/api/data");
}  // namespace StillSane::Poe::Api
