#include "fsm.hh"

#include <algorithm>
#include <functional>
#include <iostream>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/remove_if.hpp>
#include <range/v3/view/transform.hpp>
#include <sol/sol.hpp>

FSM::FSM(std::shared_ptr<sol::state> luaState) : mLuaState(luaState) {}

bool FSM::parse(const std::vector<std::string>& transition_table) {
  for (std::string transition : transition_table) {
    mTransitions.emplace_back(transition);
    if (!mTransitions.back().valid()) {
      mValid = false;
      return false;
    } else if (mTransitions.back().initial()) {
      mCurrentStates.emplace_back(mTransitions.back().src_state());
    }
  }

  // Remove duplicates states
  mCurrentStates |= ranges::actions::sort | ranges::actions::unique;

  if (mCurrentStates.empty()) {
    error("no initial state");
  }

  mValid    = true;
  mFinished = false;

  return true;
}

void FSM::process_event(const std::string& event) {
  if (!mValid)
    return;

  debug("processing event '{}'", event);

  std::vector<std::string> mNextStates;

  std::vector<std::pair<std::string, bool>> mStatesHandled =
      mCurrentStates |
      ranges::views::transform([](const std::string& s) -> std::pair<std::string, bool> {
        return {s, false};
      }) |
      ranges::to<std::vector<std::pair<std::string, bool>>>;

  for (const auto& transition : mTransitions) {
    for (auto& state : mStatesHandled) {
      if (state.second)
        break;
      if (transition.process_event(*this, event, state.first)) {
        debug("processed event '{}' successfully: {} => {}", event, state.first,
              transition.dst_state());
        state.second = true;  // Set handled
        mNextStates.push_back(transition.dst_state());
        break;
      }
    }
  }

  // Re-add unhandled states
  for (const auto& unhandled :
       mStatesHandled |
           ranges::views::remove_if([](const auto& state) { return state.second; }))
    mNextStates.push_back(unhandled.first);

  if (mCurrentStates.size() != mNextStates.size()) {
    error("mCurrentStates.size() != mNextStates.size()");
    return;
  }
  mCurrentStates = mNextStates;

  // Remove duplicates states
  mCurrentStates |= ranges::actions::sort | ranges::actions::unique;

  if (mCurrentStates.size() == 1 && mCurrentStates.at(0) == "X") {
    debug("we done boys");
    mFinished = true;
  }
}

bool FSM::finished() {
  return mFinished;
}

/*!
 * \brief FSM::execute_guard
 * \return true if the guard passed, false if the guard failed or there was an error
 */
bool FSM::execute_guard(const std::string& code) {
  // debug("executing guard '", code, "'");
  auto result = mLuaState->safe_script(code, sol::script_pass_on_error);

  if (!result.valid()) {
    sol::error err = result;
    error("failed to execute guard \"{}\"\n with error: {}", code, err.what());
    return false;
  }

  spdlog::debug("guard result: {}", static_cast<bool>(result));

  return result;
}

/*!
 * \brief FSM::execute_action
 * \return true on success, false on failure
 */
bool FSM::execute_action(const std::string& code) {
  // debug("executing action '", code, "'");

  auto result = mLuaState->safe_script(code, sol::script_pass_on_error);

  if (!result.valid()) {
    sol::error err = result;
    error("failed to execute guard: {}", err.what());
    return false;
  }

  return true;
}
