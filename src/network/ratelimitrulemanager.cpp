#include "ratelimitrulemanager.hh"

namespace AutoTrade::Network {

/*
RateLimitRuleManager::RateLimitRuleManager(const QString& rule, QObject* parent)
    : QObject(parent) {
  mTimeoutTimer.setSingleShot(true);

  if (!rule.isEmpty())
    updateRule(rule);
}

bool RateLimitRuleManager::updateRule(const QString& ruleSet) {
  auto [success, limit, interval, timeout] = extractSet(ruleSet);

  if (!success) {
    mRuleValid = false;
    return false;
  }

  mLimit    = limit;
  mInterval = interval;
  mTimeout  = timeout;

  return true;
}

bool RateLimitRuleManager::updateState(const QString& stateSet) {
  auto [success, hits, interval, activeTimeout] = extractSet(stateSet);

  if (!success) {
    return false;
  }

  mHits          = hits;
  mActiveTimeout = activeTimeout;

  return true;
}

bool RateLimitRuleManager::isValid() {
  return mRuleValid;
}

int RateLimitRuleManager::nextRequestTime() {
  int timeoutRemaining = std::max(mActiveTimeout, mTimeoutTimer.remainingTime());

  if (mIntervalTimer.remainingTime() == 0)
    mHits = 0;
  else if (mHits >= mLimit)
    return mIntervalTimer.remainingTime();

  return std::max(timeoutRemaining, mIntervalTimer.remainingTime());
}

void RateLimitRuleManager::request() {
  // Start interval timer
  if (!mIntervalTimer.isActive()) {
    mIntervalTimer.start(mInterval * 1000);
    mHits = 0;
  }

  mHits++;

  // Too many requests, start timeout remaining
  if (mHits > mLimit) {
    mHits = 0;
    mTimeoutTimer.start(mTimeout * 1000);
  }
}

std::tuple<bool, int, int, int> RateLimitRuleManager::extractSet(const QString& rule) {
  auto tokens = rule.split(":");

  if (tokens.size() != 3) {
    return {false, 0, 0, 0};
  }

  int limit, interval, timeout;

  bool ok;
  limit = tokens.at(0).toInt(&ok);
  if (!ok) {
    return {false, 0, 0, 0};
  }
  interval = 1000 * tokens.at(1).toInt(&ok);
  if (!ok) {
    return {false, 0, 0, 0};
  }
  timeout = 1000 * tokens.at(2).toInt(&ok);
  if (!ok) {
    return {false, 0, 0, 0};
  }

  return {true, limit, interval, timeout};
}
*/

}  // namespace AutoTrade::Network
