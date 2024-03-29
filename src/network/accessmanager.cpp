#include "accessmanager.hh"

#include <QElapsedTimer>
#include <QNetworkAccessManager>

#include "ratelimitmanager.hh"

namespace StillSane::Network {

AccessManager::AccessManager() {
  mNetworkAccessManager = new QNetworkAccessManager(this);
}

AccessManager::~AccessManager() {
  delete mNetworkAccessManager;
}

void AccessManager::setHeaders(QNetworkRequest& request) {
  setPOESESSID(request);
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

void AccessManager::setPOESESSID(QNetworkRequest& request) {
  QStringList cookies;
  if (request.hasRawHeader("Cookie")) {
    cookies << request.rawHeader("Cookie");
  }
  cookies << QStringLiteral("POESESSID=%1").arg(instance().mPOESESSID);
  request.setRawHeader("Cookie", cookies.join("; ").toLatin1());
}

void AccessManager::setPOESESSID(const QString& poesessid) {
  instance().mPOESESSID = poesessid;
}

void AccessManager::get(QNetworkRequest request,
                        Callback&&      slot /*, RateLimitManager* rateLimiter*/) {
  auto executeRequest = [&]() {
    QNetworkAccessManager* accessManager = instance().mNetworkAccessManager;

    setHeaders(request);
    QElapsedTimer timer;
    timer.start();
    auto reply = accessManager->get(request);
    qDebug() << "NetworkAccess::get executed, waiting for reply";

    connect(reply, &QNetworkReply::finished, [slot, reply, timer]() {
      qDebug() << "NetworkAccess::get finished in" << timer.elapsed() << "ms";
      qDebug() << "NetworkAccess::get account rates:"
               << reply->rawHeader("x-rate-limit-account");
      qDebug() << "NetworkAccess::get account rates state:"
               << reply->rawHeader("x-rate-limit-account-state");
      qDebug() << "NetworkAccess::get ip rates:" << reply->rawHeader("x-rate-limit-ip");
      qDebug() << "NetworkAccess::get ip rates state:"
               << reply->rawHeader("x-rate-limit-ip-state");
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

    setHeaders(request);
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
}

}  // namespace StillSane::Network
