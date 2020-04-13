#ifndef CURRENCYTRADER_TRADEAPI_HH
#define CURRENCYTRADER_TRADEAPI_HH

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QObject>
#include <QWebSocket>

#include <network/accessmanager.hh>
#include <network/ratelimitmanager.hh>
#include <poe/types.hh>

namespace AutoTrade::Poe {

/**
 * @brief The TradeAPI class interfaces with the official trade API at
 * https://www.pathofexile.com/api/trade.
 */
class TradeAPI : public QObject {
  Q_OBJECT

 public:
  explicit TradeAPI(QObject* parent = nullptr);
  ~TradeAPI();

  /*!
   * \brief Start a websockets search for items.
   * \param league The league in which to search.
   * \param parameters The query (search) parameters.
   */
  void openLiveSearch(const QString& cookies,
                      const QString& league,
                      const Query&   parameters);
  void closeLiveSearch();

  /*!
   * \brief Search for items.
   * \param league The league in which to search.
   * \param parameters The query (search) parameters.
   * \param slot The function called once the result is available.
   * \returns The query id.
   */
  void search(const QString& league, const Query& parameters, Network::Callback&& slot);

  /*!
   * \brief Fetches the item details.
   * \param id The item id.
   * \returns The query id.
   */
  void fetch(const QString& id, Network::Callback&& slot);
  /*!
   * \brief Fetches the items details.
   * \param ids Ids of the items.
   * \returns The query id.
   */
  void fetch(const QStringList& ids, Network::Callback&& slot);

 signals:
  void connected();
  void disconnected();

  void finished(const QUuid& id, const QByteArray& data);

  void itemReceived(const TradeItem& item);

 private:
  void onError(QAbstractSocket::SocketError error);
  void onNewMessage(const QString& message);

  QNetworkRequest buildRequest(const QString& path) const;

  /*QUuid get(const QString& path);
  QUuid post(const QString& path, const QByteArray& data);*/

  void parseFetchedItems(const QByteArray& data);

 private:
  QWebSocket mWebsocket;
  // RateLimitManager mRateLimiter;
};

}  // namespace AutoTrade::Poe

#endif  // CURRENCYTRADER_TRADEAPI_HH
