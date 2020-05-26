#pragma once

#include "module/interface/constants.hh"
#include "module/mouse/constants.hh"

namespace StillSane::Module::Mouse {

using Position_t = Interface::Position_t;

bool initialize();
void terminate();

// Position
void       move(const Position_t& position);
Position_t where();

// Button
void press(const Button& b);
void down(const Button& b);
void up(const Button& b);
bool is_down(const Button& b);
bool is_up(const Button& b);

}  // namespace StillSane::Module::Mouse
