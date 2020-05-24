#pragma once

#include <cstdint>

#include "modules/interface/constants.hh"

namespace StillSane::Modules::Mouse {

enum class Button { Left, Right };

bool initialize();
void terminate();

void move(const Interface::Position::Type& position);
void press(const Button& b);
void up(const Button& b);
void down(const Button& b);

}  // namespace StillSane::Modules::Mouse
