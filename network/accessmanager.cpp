#include "accessmanager.hh"

#include <QNetworkAccessManager>

#include <network/ratelimitmanager.hh>

namespace AutoTrade::Network {

AccessManager::AccessManager() {
  mNetworkAccessManager = new QNetworkAccessManager(this);
}

AccessManager::~AccessManager() {
  delete mNetworkAccessManager;
}

void AccessManager::modifyRequest(QNetworkRequest& request) {
  QStringList cookies;
  if (request.hasRawHeader("Cookie")) {
    cookies << request.rawHeader("Cookie");
  }
  cookies << QStringLiteral("POESESSID=%1").arg(instance().mPOESESSID);

  request.setRawHeader("Cookie", cookies.join("; ").toLatin1());
  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("TE", "Trailers");
  request.setRawHeader("X-Requested-With", "XMLHttpRequest");
  request.setRawHeader(
      "User-Agent",
      "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:75.0) Gecko/20100101 Firefox/75.0");

  if (request.url().host().contains("pathofexile.com")) {
    request.setRawHeader("Host", "www.pathofexile.com");
    request.setRawHeader("Origin", "https://www.pathofexile.com");
  }
}

void AccessManager::setPOESSID(const QString& poesessid) {
  instance().mPOESESSID = poesessid;
}

void AccessManager::get(QNetworkRequest request,
                        Callback&&      slot /*, RateLimitManager* rateLimiter*/) {
  auto executeRequest = [&]() {
    QNetworkAccessManager* accessManager = instance().mNetworkAccessManager;

    modifyRequest(request);
    auto reply = accessManager->get(request);
    qDebug() << "NetworkAccess::get executed, waiting for reply";

    connect(reply, &QNetworkReply::finished, [slot, reply]() {
      slot(reply->readAll());
      reply->deleteLater();
    });
  };

  /*
  if (rateLimiter) {
    qDebug() << "NetworkAccess::get scheduling request";
    rateLimiter->scheduleRequest(executeRequest);
  } else {*/
  qDebug() << "NetworkAccess::get executing request";
  executeRequest();
  //}
}

void AccessManager::post(QNetworkRequest   request,
                         const QByteArray& data,
                         Callback&&        slot /*, RateLimitManager* rateLimiter*/) {
  auto executeRequest = [&]() {
    QNetworkAccessManager* accessManager = instance().mNetworkAccessManager;

    modifyRequest(request);
    auto reply = accessManager->post(request, data);
    qDebug() << "NetworkAccess::post executed, waiting for reply";

    connect(reply, &QNetworkReply::finished, [slot, reply]() {
      slot(reply->readAll());
      reply->deleteLater();
    });
  };

  /*
  if (rateLimiter) {
    qDebug() << "NetworkAccess::post scheduling request";
    rateLimiter->scheduleRequest(executeRequest);
  } else {*/
  qDebug() << "NetworkAccess::post executing request";
  executeRequest();
  //}
}  // namespace AutoTrade::Network

}  // namespace AutoTrade::Network
