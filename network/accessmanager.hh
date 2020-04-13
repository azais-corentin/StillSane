#ifndef AUTOTRADE_NETWORK_NETWORKACCESS_HH
#define AUTOTRADE_NETWORK_NETWORKACCESS_HH

#include <functional>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QQueue>

namespace AutoTrade::Network {

// class RateLimitManager;

using Callback = std::function<void(const QByteArray&)>;

class AccessManager : public QObject {
  Q_OBJECT
 public:
  static AccessManager& instance() {
    static AccessManager instance;
    return instance;
  }

  AccessManager(const AccessManager&) = delete;
  AccessManager(AccessManager&&)      = delete;
  AccessManager& operator=(const AccessManager&) = delete;
  AccessManager& operator=(AccessManager&&) = delete;

  void setPOESSID(const QString& id);

 public:
  static void get(QNetworkRequest request,
                  Callback&&      slot); /*, RateLimitManager* rateLimiter = nullptr);*/

  static void post(QNetworkRequest   request,
                   const QByteArray& data,
                   Callback&& slot); /*, RateLimitManager* rateLimiter = nullptr);*/

 private:
  explicit AccessManager();
  ~AccessManager();

  static void modifyRequest(QNetworkRequest& request);

 private:
  QNetworkAccessManager* mNetworkAccessManager;

  QQueue<QNetworkRequest> mGetQueue;
};

}  // namespace AutoTrade::Network

#endif  // AUTOTRADE_NETWORK_NETWORKACCESS_HH
