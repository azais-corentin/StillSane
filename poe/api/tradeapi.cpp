#include "tradeapi.hh"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <poe/api/constants.hh>

namespace AutoTrade::Poe {

using namespace Network;

TradeAPI::TradeAPI(QObject* parent) : QObject(parent) {
  // Connect REST API
  // connect(mNetworkManager, &QNetworkAccessManager::finished, this,
  // &TradeAPI::onFinished);

  // Connect Websocket
  connect(&mWebsocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
          this, &TradeAPI::onError);
  connect(&mWebsocket, &QWebSocket::connected, this, &TradeAPI::connected);
  connect(&mWebsocket, &QWebSocket::disconnected, this, &TradeAPI::disconnected);
  connect(&mWebsocket, &QWebSocket::textMessageReceived, this, &TradeAPI::onNewMessage);
}

TradeAPI::~TradeAPI() {
  // delete mNetworkManager;
}

void TradeAPI::openLiveSearch(const QString& cookies,
                              const QString& league,
                              const Query&   parameters) {
  search(league, parameters, [this, cookies](const QByteArray& data) {
    const QString liveSearchId = QJsonDocument::fromJson(data).object()["id"].toString();

    QNetworkRequest connection("wss://www.pathofexile.com/api/trade/live/" +
                               currentLeague + "/" + liveSearchId);

    connection.setRawHeader("Host", "www.pathofexile.com");
    connection.setRawHeader("Origin", "https://www.pathofexile.com");
    connection.setRawHeader("Cookie", cookies.toUtf8());

    qDebug() << connection.url();

    mWebsocket.open(connection);
  });
}

void TradeAPI::closeLiveSearch() {
  mWebsocket.close();
}

void TradeAPI::search(const QString& league, const Query& parameters, Callback&& slot) {
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

void TradeAPI::fetch(const QString& id, Callback&& slot) {
  fetch(QStringList{id}, std::forward<Callback>(slot));
}

void TradeAPI::fetch(const QStringList& ids, Callback&& slot) {
  // /fetch/  id1,...,idn
  QString idQuery = ids.join(',');

  AccessManager::get(buildRequest(TradeFetchPath + idQuery),
                     std::forward<Callback>(slot)); /*, &mRateLimiter);*/
}

void TradeAPI::onError(QAbstractSocket::SocketError) {
  qDebug() << "TradeAPI::onError websockets:" << mWebsocket.errorString();
}

void TradeAPI::onNewMessage(const QString& message) {
  const auto& newItems =
      QJsonDocument::fromJson(message.toUtf8()).object().value("new").toArray();

  QStringList ids;

  for (const auto& item : newItems) {
    ids << item.toString();
  }

  fetch(ids, [&](const QByteArray& data) { parseFetchedItems(data); });
}

/*!
 * \brief Builds a request from path.
 *
 * \param path The path of the request.
 * \return A network request for the path with the proper headers.
 */
QNetworkRequest TradeAPI::buildRequest(const QString& path) const {
  QUrl url;
  url.setScheme("https");
  url.setHost(baseHostOfficial);
  url.setPath(basePathOfficial + path);

  QNetworkRequest request;
  request.setUrl(url);

  return request;
}

void TradeAPI::parseFetchedItems(const QByteArray& data) {
  const QJsonArray results = QJsonDocument::fromJson(data).object()["result"].toArray();

  for (const auto& result : results) {
    const QJsonObject item  = result["item"].toObject();
    const QJsonObject price = result["listing"].toObject()["price"].toObject();

    const QString name = item["name"].toString();
    const QString type = item["typeLine"].toString();

    emit itemReceived(
        {name + type, name, type, price["amount"].toInt(), price["currency"].toString()});
  }
}

}  // namespace AutoTrade::Poe
