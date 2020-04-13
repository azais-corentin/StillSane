#include "ninjaapi.hh"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include <network/accessmanager.hh>
#include <poe/constants.hh>

using namespace std::placeholders;

namespace AutoTrade::Poe {

using namespace Network;

NinjaAPI::NinjaAPI(QObject* parent) : QObject(parent) {
  // Fetch currency equivalents by default
  fetchCurrencyOverview(currentLeague, "Currency");
}

double NinjaAPI::getChaosEquivalent(const QString& tradeId) {
  if (mUpToDateCurrencyDetail) {
    auto it = std::find_if(
        mCurrencyNames.begin(), mCurrencyNames.end(),
        [tradeId](const CurrencyDetail& detail) { return detail.tradeId == tradeId; });

    if (it != mCurrencyNames.end()) {
      return it->chaosEquivalent;
    }
  } else {
    if (!mUpdatingCurrencyDetail)
      fetchCurrencyOverview(currentLeague, "Currency");
  }
  return 0.0;
}

void NinjaAPI::fetchCurrencyOverview(const QString& league, const QString& currencyType) {
  mUpdatingCurrencyDetail = true;

  auto      request = buildRequest("/currencyoverview");
  QUrlQuery query{{"league", league}, {"type", currencyType}, {"language", "en"}};

  QUrl url = request.url();
  url.setQuery(query);
  request.setUrl(url);

  AccessManager::get(request, std::bind(&NinjaAPI::parseCurrencyOverview, this, _1));
}

void NinjaAPI::fetchCurrencyHistory(const QString&, const QString&, unsigned int) {
  /*!
   * \todo Implement this
   */
}

void NinjaAPI::fetchItemOverview(const QString&, const QString&) {}

void NinjaAPI::fetchItemHistory(const QString&, const QString&, unsigned int) {
  /*!
   * \todo Implement this
   */
}

void NinjaAPI::parseCurrencyOverview(const QByteArray& data) {
  const auto& results             = QJsonDocument::fromJson(data);
  const auto& currencyDetails     = results["currencyDetails"].toArray();
  const auto& currencyEquivalents = results["lines"].toArray();

  for (const auto& detail : currencyDetails) {
    mCurrencyNames.push_back({detail["name"].toString(), detail["tradeId"].toString(),
                              detail["id"].toInt(), 0.0});
  }

  for (const auto& equivalent : currencyEquivalents) {
    const QString name = equivalent["currencyTypeName"].toString();
    auto          it   = std::find_if(
        mCurrencyNames.begin(), mCurrencyNames.end(),
        [name](const CurrencyDetail& detail) { return detail.name == name; });

    if (it != mCurrencyNames.end()) {
      it->chaosEquivalent = equivalent["chaosEquivalent"].toDouble();
    }
  }

  // Special case for Chaos Orb: set to 1.0 chaosEquivalent
  auto it = std::find_if(
      mCurrencyNames.begin(), mCurrencyNames.end(),
      [](const CurrencyDetail& detail) { return detail.name == "Chaos Orb"; });
  if (it != mCurrencyNames.end()) {
    it->chaosEquivalent = 1.0;
  }

  mUpToDateCurrencyDetail = true;
  mUpdatingCurrencyDetail = false;
}

void NinjaAPI::parseCurrencyHistory(const QByteArray&) {}

void NinjaAPI::parseItemOverview(const QByteArray&) {}

void NinjaAPI::parseItemHistory(const QByteArray&) {}

QNetworkRequest NinjaAPI::buildRequest(const QString& path) const {
  QUrl url;
  url.setScheme("https");
  url.setHost(baseHostNinja);
  url.setPath(basePathNinja + path);

  QNetworkRequest request;
  request.setUrl(url);
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Host", baseHostNinja.toLocal8Bit());

  return request;
}

}  // namespace AutoTrade::Poe
