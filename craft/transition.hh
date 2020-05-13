#pragma once

#include <iostream>
#include <string>
#include <tuple>

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
  void error(const Args&... args) const {
    std::cerr << "Error: ";
    (std::cerr << ... << args) << "\n";
  }

  template <typename... Args>
  void debug(const Args&... args) const {
    (std::cout << ... << args) << "\n";
  }

 private:
  bool mValid   = false;
  bool mInitial = false;

  std::string mSrcState, mEvent, mGuard, mAction, mDstState;

  bool mHasEvent, mHasGuard, mHasAction;
};
