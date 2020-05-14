#pragma once

#include <string>
#include <tuple>

#include <spdlog/spdlog.h>

class FSM;

class Transition {
 public:
  Transition(std::string transition);

  bool valid() const;
  bool initial() const;

  const std::string& guard_code() const;
  const std::string& actions_code() const;

  bool process_event(FSM& fsm, const std::string& event, const std::string& state) const;

  std::string src_state() const;
  std::string dst_state() const;

 private:
  void convertScripts();

  template <typename... Args>
  void error(Args&&... args) const {
    spdlog::error(std::forward<Args>(args)...);
  }

  template <typename... Args>
  void debug(Args&&... args) const {
    spdlog::debug(std::forward<Args>(args)...);
  }

 private:
  bool mValid   = false;
  bool mInitial = false;

  bool        mHasEvent, mHasGuard, mHasAction;
  std::string mSrcState, mEvent, mGuard, mAction, mDstState;
};
