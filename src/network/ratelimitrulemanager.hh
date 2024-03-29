#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

namespace StillSane::Network {

/*
class RateLimitRuleManager : public QObject {
  Q_OBJECT
 public:
  explicit RateLimitRuleManager(const QString& rule = "", QObject* parent = nullptr);

  bool updateRule(const QString& ruleSet);
  bool updateState(const QString& stateSet);

  bool isValid();

  int  nextRequestTime();
  void request();

 private:
  std::tuple<bool, int, int, int> extractSet(const QString& rule);

 private:
  int mLimit = 0, mInterval = 0, mTimeout = 0;
  int mHits = 0, mActiveTimeout = 0;

  bool   mRuleValid = false;
  QTimer mIntervalTimer, mTimeoutTimer;
};
*/

}  // namespace StillSane::Network
