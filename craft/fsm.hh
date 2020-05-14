#pragma once

#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <sol/forward.hpp>

#include "transition.hh"

class FSM {
 public:
  FSM(std::shared_ptr<sol::state> luaState);

  bool parse(const std::vector<std::string>& transition_table);

  void process_event(const std::string& event);
  bool finished();

  bool execute_guard(const std::string& code);
  bool execute_action(const std::string& code);

 private:
  template <typename... Args>
  void error(Args&&... args) const {
    spdlog::error(std::forward<Args>(args)...);
  }

  template <typename... Args>
  void debug(Args&&... args) const {
    spdlog::debug(std::forward<Args>(args)...);
  }

 private:
  bool                        mValid = false, mFinished = false;
  std::shared_ptr<sol::state> mLuaState;

  std::vector<std::string> mCurrentStates;

  std::vector<Transition> mTransitions;
};
