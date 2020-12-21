#pragma once

#include <cstdint>

#include <module/interface/constants.hh>

namespace StillSane::Module {

class Mouse {
 public:
  using Position_t = Interface::Position_t;
  enum class Button : uint8_t { Left, Right };

 public:
  Mouse();
  ~Mouse();

  // Position
  void       move(const Position_t& position);
  Position_t where();

  // Button
  void press(const Button& b);
  void down(const Button& b);
  void up(const Button& b);
  bool is_down(const Button& b);
  bool is_up(const Button& b);

 private:
  class MouseImplementation;
  MouseImplementation* mImpl;
};

}  // namespace StillSane::Module
