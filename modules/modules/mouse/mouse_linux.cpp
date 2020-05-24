#include "mouse.hh"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <spdlog/spdlog.h>

/**
 * Linux implementation of the mouse module
 */
namespace StillSane::Modules::Mouse {

inline Display* mainDisplay = nullptr;

bool initialize() {
  mainDisplay = XOpenDisplay(nullptr);

  if (!mainDisplay) {
    spdlog::error("Mouse::initialize error: unable to open open display");
  }

  return true;
}

void terminate() {
  if (mainDisplay)
    XCloseDisplay(displayMain);
}

void move(const Interface::Position::Type& position) {
  XWarpPointer(mainDisplay, None, None, 0, 0, 0, 0, position.x, position.y);
}

void press(const Mouse::Button& /*b*/) {
  spdlog::error("Mouse::press: not implemented on linux");
}

void down(const Mouse::Button& /*b*/) {
  spdlog::error("Mouse::down: not implemented on linux");
}

void up(const Mouse::Button& /*b*/) {
  spdlog::error("Mouse::up: not implemented on linux");
}


}  // namespace StillSane::Modules
