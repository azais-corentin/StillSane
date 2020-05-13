#include "fsm.hh"

#include <algorithm>
#include <functional>
#include <iostream>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/for_each.hpp>

FSM::FSM() {}

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

  mValid = true;
  return true;
}

void FSM::process_event(const std::string& event) {
  if (!mValid)
    return;

  debug("processing event '", event, "'");

  std::vector<std::string> mNextStates;

  std::vector<std::pair<std::string, bool>> mStatesHandled;
  for (const auto& state : mCurrentStates)
    mStatesHandled.emplace_back(state, false);

  for (const auto& transition : mTransitions) {
    for (auto& state : mStatesHandled) {
      if (state.second)
        break;
      if (transition.process_event(*this, event, state.first)) {
        debug("processing ", event, " successfully: ", state.first, " => ",
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
  }
}

bool FSM::execute_guard(const std::string& code) {
  debug("executing guard '", code, "'");
  return true;  // \todo execute lua code
}

bool FSM::execute_action(const std::string& code) {
  debug("executing action '", code, "'");
  return true;
}
