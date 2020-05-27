#include "trade.hh"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <utility>

#include "constants.hh"

namespace StillSane::Poe::Api {

using namespace Network;

Trade::Trade(QObject* parent) : QObject(parent) {
  // Connect Websocket
  connect(&mWebsocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
          this, &Trade::onError);
  connect(&mWebsocket, &QWebSocket::connected, [&]() { emit connectionChanged(true); });
  connect(&mWebsocket, &QWebSocket::disconnected,
          [&]() { emit connectionChanged(false); });
  connect(&mWebsocket, &QWebSocket::textMessageReceived, this, &Trade::onNewMessage);
}

Trade::~Trade() {
  closeLiveSearch();
}

auto Trade::isEnabled() const -> bool {
  return mEnabled;
}

auto Trade::isConnected() const -> bool {
  return mWebsocket.state() == QAbstractSocket::ConnectedState;
}

auto Trade::getId() const -> QString {
  return mId;
}

auto Trade::getLeague() const -> QString {
  return mLeague;
}

auto Trade::getName() const -> QString {
  return mName;
}

void Trade::setEnabled(bool enabled) {
  if (mEnabled == enabled) {
    return;
  }

  mEnabled = enabled;
  emit enabledChanged(mEnabled);

  if (mEnabled) {
    openLiveSearch();
  } else {
    closeLiveSearch();
  }
}

void Trade::setId(QString id) {
  mId = std::move(id);
}

void Trade::setLeague(QString league) {
  mLeague = std::move(league);
}

void Trade::setName(QString name) {
  mName = std::move(name);
}

void Trade::openLiveSearch(const QString& league, const Query& parameters) {
  search(league, parameters, [this, league](const QByteArray& data) {
    const QString liveSearchId = QJsonDocument::fromJson(data).object()["id"].toString();

    setId(liveSearchId);
    setLeague(league);

    openLiveSearch();
  });
}

void Trade::openLiveSearch(const QString& searchUrl) {
  auto&& split  = searchUrl.splitRef("/");
  auto&& id     = split.takeLast();
  auto&& league = split.takeLast();

  setId(id.toString());
  setLeague(league.toString());

  openLiveSearch();
}

void Trade::openLiveSearch() {
  if (isConnected()) {
    closeLiveSearch();
  }

  QNetworkRequest connection(
      QStringLiteral("wss://www.pathofexile.com/api/trade/live/%1/%2")
          .arg(mLeague)
          .arg(mId));

  Network::AccessManager::setPOESESSID(connection);
  connection.setRawHeader("Host", "www.pathofexile.com");
  connection.setRawHeader("Origin", "https://www.pathofexile.com");
  /*connection.setRawHeader("Upgrade", "websocket");
  connection.setRawHeader(
      "User-Agent",
      "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:75.0) Gecko/20100101 Firefox/75.0");
  connection.setRawHeader("Pragma", "no-cache");
  connection.setRawHeader("Cache-Control", "no-cache");*/

  qDebug() << connection.rawHeaderList();

  qDebug() << connection.rawHeader("Cookie");

  qDebug() << "Opened live search for url:" << connection.url();

  mWebsocket.open(connection);
}

void Trade::closeLiveSearch() {
  mWebsocket.close();
}

void Trade::search(const QString& league, const Query& parameters, Callback&& slot) {
  // path: /search/  currentLeague
  const auto request = buildRequest(TradeSearchPath + league);

  QJsonObject data;
  data["query"] = QJsonObject{
      {"status", QJsonObject{{"option", "online"}}},
      {"name", parameters.name},
      {"type", parameters.type},
      {"filters",
       QJsonObject{{"trade_filters",
                    QJsonObject{{"filters",
                                 QJsonObject{{"sale_type",
                                              QJsonObject{{"option", "priced"}}}}}}}}}};
  data["sort"] = QJsonObject{{"price", "asc"}};

  AccessManager::post(request, QJsonDocument{data}.toJson(QJsonDocument::Compact),
                      std::forward<Callback>(slot)); /*, &mRateLimiter);*/
}

void Trade::fetch(const QString& id, Callback&& slot) {
  fetch(QStringList{id}, std::forward<Callback>(slot));
}

void Trade::fetch(const QStringList& ids, Callback&& slot) {
  // /fetch/  id1,...,idn with up to 20 ids
  /*!
   * \todo Paginate the fetch requests if ids.size() > 20.
   */
  QString idQuery = ids.join(',');

  AccessManager::get(buildRequest(TradeFetchPath + idQuery),
                     std::forward<Callback>(slot)); /*, &mRateLimiter);*/
}

void Trade::onError(QAbstractSocket::SocketError /*unused*/) {
  qDebug() << "TradeAPI::onError websockets:" << mWebsocket.errorString();
}

void Trade::onNewMessage(const QString& message) {
  const auto& newItems =
      QJsonDocument::fromJson(message.toUtf8()).object().value("new").toArray();

  if (newItems.isEmpty()) {
    return;
  }

  QStringList ids;

  for (const auto& item : newItems) {
    ids << item.toString();
  }

  qDebug() << ids;

  fetch(ids, [&](const QByteArray& data) { parseFetchedItems(data); });
}

/*!
 * \brief Builds a request from path.
 *
 * \param path The path of the request.
 * \return A network request for the path with the proper headers.
 */
auto Trade::buildRequest(const QString& path) -> QNetworkRequest {
  QUrl url;
  url.setScheme("https");
  url.setHost(baseHostOfficial);
  url.setPath(basePathOfficial + path);

  QNetworkRequest request;
  request.setUrl(url);

  return request;
}

void Trade::parseFetchedItems(const QByteArray& data) {
  const QJsonArray results = QJsonDocument::fromJson(data).object()["result"].toArray();

  for (const auto& result : results) {
    const QJsonObject item  = result["item"].toObject();
    const QJsonObject price = result["listing"].toObject()["price"].toObject();

    const QString name = item["name"].toString();
    const QString type = item["typeLine"].toString();

    qDebug() << "new item!";

    emit itemReceived(
        {name + type, name, type, price["amount"].toInt(), price["currency"].toString()});
  }
}

}  // namespace StillSane::Poe::Api
