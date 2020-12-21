#pragma once

#include <module/mouse/mouse.hh>

namespace StillSane::Module {

/**
 * Windows implementation of the mouse module
 */
class Mouse::MouseImplementation {
 public:
  MouseImplementation();
  ~MouseImplementation();

  // Position
  void              move(const Position_t& position);
  Mouse::Position_t where();

  // Button
  void press(const Button& b);
  void down(const Button& b);
  void up(const Button& b);
  bool is_down(const Button& b);
  bool is_up(const Button& b);

 private:
  int get_key(const Button& b);

 private:
  bool mButtonsSwapped;
};

}  // namespace StillSane::Module
