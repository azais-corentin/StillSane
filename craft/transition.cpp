#include "transition.hh"

#include <functional>

#include "fsm.hh"

void replace_all_inplace(std::string&       str,
                         const std::string& from,
                         const std::string& to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

bool for_each_token(std::string&                           string,
                    const std::string&                     tokens,
                    std::function<bool(std::string, char)> callback) {
  using size_t = std::string::size_type;
  size_t pos   = 0;

  std::vector<size_t> ops_positions;

  while (pos = string.find_first_of(tokens, pos), pos != std::string::npos) {
    ops_positions.emplace_back(pos);
    pos++;
  }

  size_t offset       = 0;
  char   last_element = ' ';
  for (const auto& op_position : ops_positions) {
    if (!callback(string.substr(offset, op_position - offset), last_element))
      return false;
    last_element = string.at(op_position);
    offset       = op_position + 1;
  }
  if (!callback(string.substr(offset), last_element))
    return false;

  return true;
}

Transition::Transition(std::string transition) {
  mValid = false;
  debug("Parsing transition: {}", transition);
  replace_all_inplace(transition, " ", "");

  bool has_src_state = false, has_event = false, has_guard = false, has_action = false,
       has_dst_state = false;

  bool line_parsed = for_each_token(
      transition, "+[/=", [&](const std::string& element, const char& token) {
        bool success = true;
        switch (token) {
          case ' ':
            if (has_src_state) {
              error("okay this shouldn't be possible, just saying");
              success = false;
            } else {
              has_src_state = true;
              if (element.starts_with("*")) {
                std::string initial_state = element.substr(1);
                mInitial                  = true;
                mSrcState                 = initial_state;
                // debug("initial state: ", mSrcState);
              } else {
                mSrcState = element;
                // debug("source state: ", mSrcState);
              }
            }
            break;
          case '+':
            if (has_event) {
              // Multiple events
              error("invalid event ('+') provided before '{}'", element);
              success = false;
            } else {
              if (has_dst_state || has_action || has_guard) {
                // Wrong order
                error("wrong order for event '{}'", element);
                success = false;
              } else {
                has_event = true;
                mEvent    = element;
                // debug("event: ", mEvent);
              }
            }
            break;
          case '[':
            if (has_guard) {
              error("invalid guard ('[]') provided before '{}'", element);
              success = false;
            } else {
              mGuard = element;
              mGuard.pop_back();
              if (!element.ends_with(']')) {
                error("expected ']' after guard '{}'", element);
                success = false;
              } else if (has_dst_state || has_action) {
                error("wrong order for guard '{}'", mGuard);
                success = false;
              } else {
                has_guard = true;
                // debug("guard: ", mGuard);
              }
            }
            break;
          case '/':
            if (has_action) {
              error("unexpected action ('/') provided before '{}'", element);
              success = false;
            } else if (has_dst_state) {
              error("unexpected action '{}' provided after destination state", element);
              success = false;
            } else {
              has_action = true;
              mAction    = element;
              // debug("action: ", mAction);
            }
            break;
          case '=':
            if (has_dst_state) {
              error("unexpected destination state ('=') provided before '{}'", element);
              success = false;
            } else {
              has_dst_state = true;
              mDstState     = element;
              // debug("destination state: ", mDstState);
            }
            break;
        }

        return success;
      });

  if (!line_parsed) {
    return;
  }

  if (!has_src_state) {
    error("expected source state");
    return;
  } else if (!has_dst_state) {
    error("expected destination state");
    return;
  }

  mHasEvent  = has_event;
  mHasGuard  = has_guard;
  mHasAction = has_action;

  convertScripts();

  mValid = true;
}

bool Transition::valid() const {
  return mValid;
}

bool Transition::initial() const {
  return mInitial;
}

const std::string& Transition::guard_code() const {
  return mGuard;
}

const std::string& Transition::actions_code() const {
  return mAction;
}

bool Transition::process_event(FSM&               fsm,
                               const std::string& event,
                               const std::string& state) const {
  if (!valid())
    return false;
  if (state != mSrcState)
    return false;
  if (mHasEvent && mEvent != event)
    return false;
  if (!mHasGuard || fsm.execute_guard(mGuard)) {
    // If has no guard or guard passed
    if (mHasAction && !fsm.execute_action(mAction)) {
      // If has an action and it failed
      error("failed to execute action: '{}'", mAction);
      return false;
    }
    return true;
  }

  return false;
}

std::string Transition::src_state() const {
  return mSrcState;
}

std::string Transition::dst_state() const {
  return mDstState;
}

void Transition::convertScripts() {
  if (mHasGuard) {
    // syntax before: guard1&&!guard2
    replace_all_inplace(mGuard, "!", "not ");
    replace_all_inplace(mGuard, "&&", "() and ");
    replace_all_inplace(mGuard, "||", " or ");
    mGuard.append("() )");
    mGuard.insert(0, "return ( ");
    // syntax after: return ( guard1() and not guard2() )
  }

  if (mHasAction) {
    // syntax before: (action1,action2)
    replace_all_inplace(mAction, "(", "");
    replace_all_inplace(mAction, ")", "");
    replace_all_inplace(mAction, ",", "() ");
    mAction.append("()");
    // syntax after: action1() action2()
  }
}
