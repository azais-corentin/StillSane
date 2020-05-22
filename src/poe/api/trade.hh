#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QObject>
#include <QWebSocket>

#include "../network/accessmanager.hh"
#include "../network/ratelimitmanager.hh"
#include "types.hh"

namespace StillSane::Poe::Api {

/**
 * @brief The Trade class interfaces with the official trade API at
 * https://www.pathofexile.com/api/trade.
 */
class Trade : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged MEMBER mEnabled)
  Q_PROPERTY(bool connected READ isConnected NOTIFY connectionChanged)
  Q_PROPERTY(QString id READ getId WRITE setId MEMBER mId)
  Q_PROPERTY(QString league READ getLeague WRITE setLeague MEMBER mLeague)
  Q_PROPERTY(QString name READ getName WRITE setName MEMBER mName)

 public:
  explicit Trade(QObject* parent = nullptr);
  ~Trade();

  bool    isEnabled() const;
  bool    isConnected() const;
  QString getId() const;
  QString getLeague() const;
  QString getName() const;

  void setEnabled(bool enabled);
  void setId(QString id);
  void setLeague(QString league);
  void setName(QString name);

 public:
  /*!
   * \brief Starts a websockets live search for items.
   * \param league The league in which to search.
   * \param parameters The query (search) parameters.
   */
  void openLiveSearch(const QString& league, const Query& parameters);
  /*!
   * \brief Stats a websockets live search from the search url.
   * \param id Url or id of the search (ie.
   * https://www.pathofexile.com/trade/search/Delirium/NK6Ec5 or Delirium/NK6Ec5).
   */
  void openLiveSearch(const QString& searchUrl);
  void openLiveSearch();
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
  void itemReceived(const TradeItem& item);
  void enabledChanged(bool enabled);
  void connectionChanged(bool connected);

 private:
  void onError(QAbstractSocket::SocketError error);
  void onNewMessage(const QString& message);

  QNetworkRequest buildRequest(const QString& path) const;

  /*QUuid get(const QString& path);
  QUuid post(const QString& path, const QByteArray& data);*/

  void parseFetchedItems(const QByteArray& data);

 private:
  QWebSocket mWebsocket;

  bool    mEnabled = true;
  QString mLeague, mId, mName;

  // RateLimitManager mRateLimiter;
};

}  // namespace StillSane::Poe::Api
