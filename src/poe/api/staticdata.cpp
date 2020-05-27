#include "staticdata.hh"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "../../network/accessmanager.hh"
#include "constants.hh"

namespace StillSane::Poe::Api {

using namespace Network;

StaticData::StaticData(QObject* parent) : QObject(parent) {
  fetchItems();
  fetchStats();
  fetchStatic();
}

StaticData::~StaticData() = default;

void StaticData::fetchItems() {
  qDebug() << "Fetching static data for items";
  using namespace std::placeholders;

  AccessManager::get(buildRequest(StaticBasePath + StaticItemsPath),
                     [this](auto&& PH1) { parseFetchedItems(PH1); });
}

void StaticData::fetchStats() {
  qDebug() << "Fetching static data for stats";
  using namespace std::placeholders;

  AccessManager::get(buildRequest(StaticBasePath + StaticStatsPath),
                     [this](auto&& data) { parseFetchedStats(data); });
}

void StaticData::fetchStatic() {
  qDebug() << "Fetching static data for static";
  using namespace std::placeholders;

  AccessManager::get(buildRequest(StaticBasePath + StaticStaticPath),
                     [this](auto&& data) { parseFetchedStatic(data); });
}

void StaticData::parseFetchedItems(const QByteArray& data) {
  qDebug() << data;
  qDebug() << "Parsing static data for items:" << data.size() << "bytes";
  mItems.clear();

  const auto& categories = QJsonDocument::fromJson(data)["result"].toArray();

  for (const auto& category : categories) {
    const auto& entries = category["entries"].toArray();
    for (const auto& entry : entries) {
      auto&& name = entry["name"].toString();
      auto&& type = entry["type"].toString();
      auto&& text = entry["text"].toString();
      mItems.push_back({text, name, type});
    }
  }

  emit itemsFetched(mItems);

  qDebug() << "Parsed" << mItems.size() << "items";
}

void StaticData::parseFetchedStats(const QByteArray& data) {
  qDebug() << "Parsing static data for stats:" << data.size() << "bytes";
}

void StaticData::parseFetchedStatic(const QByteArray& data) {
  qDebug() << "Parsing static data for static:" << data.size() << "bytes";
}

auto StaticData::buildRequest(const QString& path) -> QNetworkRequest {
  QUrl url;
  url.setScheme("https");
  url.setHost(baseHostOfficial);
  url.setPath(basePathOfficial + path);

  QNetworkRequest request;
  request.setUrl(url);

  return request;
}

}  // namespace StillSane::Poe::Api
