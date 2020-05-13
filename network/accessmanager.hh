#pragma once

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

  static void setPOESESSID(QNetworkRequest& request);
  static void setPOESESSID(const QString& poesessid);

 public:
  static void get(QNetworkRequest request,
                  Callback&&      slot); /*, RateLimitManager* rateLimiter = nullptr);*/

  static void post(QNetworkRequest   request,
                   const QByteArray& data,
                   Callback&& slot); /*, RateLimitManager* rateLimiter = nullptr);*/

 private:
  explicit AccessManager();
  ~AccessManager();

  static void setHeaders(QNetworkRequest& request);

 private:
  QNetworkAccessManager* mNetworkAccessManager;

  QQueue<QNetworkRequest> mGetQueue;
  QString                 mPOESESSID;
};

}  // namespace AutoTrade::Network
