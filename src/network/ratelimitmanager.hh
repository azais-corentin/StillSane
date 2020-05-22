#pragma once

#include <functional>
#include <memory>

#include <QObject>
#include <QQueue>
#include <QTimer>

#include "ratelimitrulemanager.hh"

namespace StillSane::Network {

/**
 * @brief The RateLimitManager class
 *
 * Explanation of the headers: https://www.pathofexile.com/forum/view-thread/2079853
 *
 * Headers:
 *
 * The "X-Rate-Limit-{Method}" format is a comma delimited list (,) of rate limiting
 * rules, which themselves are delimited by colons (:). In this case there is only one
 * rule so there are no commas.
 *
 * Each rule is made up of three parts: request limit, interval (in seconds), timeout (in
 * seconds). For the trade limits, this means that you will be rate-limited for 60 seconds
 * if you make more than 20 requests in 5 seconds.
 *
 * The "X-Rate-Limit-{Method}-State" format is similar (comma delimited then colon
 * delimited), except shows the state for your connection.
 *
 * It's made up of: current hits, interval (the same as above), active timeout (in
 * seconds). For the example you gave this means that you've made 2 requests towards the 5
 * second interval and are currently rate-limited for 0 seconds (ie. you're not limited).
 *
 * Other headers related to rate-limiting include:
 *
 * The "X-Rate-Limit-Policy" header shows the policy that's active on the API endpoint
 * (searches/exchange queries use the same policy) so requests to either of these
 * endpoints will contribute to the same rate-limit rules.
 *
 * The "X-Rate-Limit-Rules" header is just a comma delimited list of methods (as in
 * "X-Rate-Limit-{Method}").
 *
 * We also provide a "Retry-After" header that gives the time you're rate-limited for in
 * seconds (if you are rate-limited).
 *
 * Including your session ID:
 *
 * Including your session ID ("logged in") will change how certain policies track your
 * limits. In the case of the trade search policy, logging in will rate-limit you based on
 * your account instead of your IP (with the same limits) and the rate-limit based on your
 * IP will allow double the requests per interval. You can see this change in the headers
 * if you try making requests with your session ID.
 */
/*
class RateLimitManager : public QObject {
  Q_OBJECT
 public:
  explicit RateLimitManager(QObject* parent = nullptr);

  bool updateRules(const QString& rules);
  bool updateStates(const QString& states);

  void scheduleRequest(std::function<void()> request);

  int nextRequestTime();

 private:
  void executeNextRequest();
  void scheduleNextRequest();

 private:
  QVector<std::unique_ptr<RateLimitRuleManager>> mRules;
  QQueue<std::function<void()>>                  mScheduledRequests;

  QTimer mTimerNextRequest;
};
*/

}  // namespace StillSane::Network
