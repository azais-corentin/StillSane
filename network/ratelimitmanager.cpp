#include "ratelimitmanager.hh"

/*
namespace AutoTrade::Network {

RateLimitManager::RateLimitManager(QObject* parent) : QObject(parent) {
  mTimerNextRequest.setSingleShot(true);

  connect(&mTimerNextRequest, &QTimer::timeout, this,
          &RateLimitManager::executeNextRequest);
}

bool RateLimitManager::updateRules(const QString& rules) {
  QStringList ruleSets = rules.split(",");
  int         n        = ruleSets.size();

  mRules.resize(n);

  bool success = true;

  for (int i = 0; i < n; i++) {
    if (i >= mRules.size()) {
      mRules.push_back(std::make_unique<RateLimitRuleManager>(ruleSets[i]));
      if (!mRules.last()->isValid())
        success = false;
    } else if (!mRules[i]->updateRule(ruleSets[i]))
      success = false;
  }

  return success;
}

bool RateLimitManager::updateStates(const QString& states) {
  QStringList stateSets = states.split(",");
  int         n         = stateSets.size();

  mRules.resize(n);

  bool success = true;

  for (int i = 0; i < n; i++) {
    if (!mRules[i]->updateState(stateSets[i]))
      success = false;
  }

  return success;
}

void RateLimitManager::scheduleRequest(std::function<void()> request) {
  mScheduledRequests.enqueue(request);

  if (mScheduledRequests.size() == 1) {
    scheduleNextRequest();
  }
}

int RateLimitManager::nextRequestTime() {
  int nextTime = 0;
  for (auto&& rule : mRules) {
    nextTime = std::max(nextTime, rule->nextRequestTime());
  }
  return nextTime;
}

void RateLimitManager::executeNextRequest() {
  if (!mScheduledRequests.isEmpty()) {
    mScheduledRequests.dequeue()();
    scheduleNextRequest();
  }
}

void RateLimitManager::scheduleNextRequest() {
  mTimerNextRequest.start(nextRequestTime());
}

}  // namespace AutoTrade::Network
*/
