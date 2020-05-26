#include "module/mouse/mouse.hh"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <spdlog/spdlog.h>

/**
 * Linux implementation of the mouse module
 */
namespace StillSane::Module::Mouse {

inline Display* mainDisplay = nullptr;

bool initialize() {
  mainDisplay = XOpenDisplay(nullptr);

  if (!mainDisplay) {
    spdlog::error("Mouse::initialize error: unable to open open display");
    return false;
  }

  return true;
}

void terminate() {
  if (mainDisplay)
    XCloseDisplay(displayMain);
}

void move(const Position_t& position) {
  XWarpPointer(mainDisplay, None, None, 0, 0, 0, 0, position.x, position.y);
}

Position_t where() {
  spdlog::error("Mouse::where: not implemented on linux");
  return {};
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

bool is_down(const Button& b) {
  spdlog::error("Mouse::up: not implemented on linux");
  return false;
}

bool is_up(const Button& b) {
  spdlog::error("Mouse::up: not implemented on linux");
  return false;
}

}  // namespace StillSane::Module
