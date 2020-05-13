#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "transition.hh"

class FSM {
 public:
  FSM();

  bool parse(const std::vector<std::string>& transition_table);

  void process_event(const std::string& event);

  bool execute_guard(const std::string& code);
  bool execute_action(const std::string& code);

 private:
  template <typename... Args>
  void error(const Args&... args) {
    std::cerr << "Error: ";
    (std::cerr << ... << args) << "\n";
  }

  template <typename... Args>
  void debug(const Args&... args) {
    (std::cout << ... << args) << "\n";
  }

 private:
  bool mValid = false;

  std::vector<std::string> mCurrentStates;

  std::vector<Transition> mTransitions;
};
