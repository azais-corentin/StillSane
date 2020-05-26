#pragma once

#include "module/interface/constants.hh"
#include "module/mouse/constants.hh"

namespace StillSane::Module::Mouse {

bool initialize();
void terminate();

void move(const Interface::Position::Type& position);
void press(const Button& b);
void up(const Button& b);
void down(const Button& b);

}  // namespace StillSane::Module::Mouse
