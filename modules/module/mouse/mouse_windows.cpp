#include "module/mouse/mouse.hh"

#include <windows.h>

/**
 * Windows implementation of the mouse module
 */
namespace StillSane::Module::Mouse {

bool initialize() {
  return true;
}

void terminate() {}

void move(const Interface::Position::Type& position) {
  INPUT ip;

  ip.type         = INPUT_MOUSE;
  ip.mi.mouseData = 0;
  ip.mi.time      = 0;
  ip.mi.dx        = position.x * (65536 / GetSystemMetrics(SM_CXSCREEN));
  ip.mi.dy        = position.y * (65536 / GetSystemMetrics(SM_CYSCREEN));
  ip.mi.dwFlags   = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &ip, sizeof(ip));
}

void press(const Mouse::Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left)
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
  else if (b == Button::Right)
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;

  SendInput(1, &ip, sizeof(ip));
}

void down(const Mouse::Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left)
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  else if (b == Button::Right)
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

  SendInput(1, &ip, sizeof(ip));
}

void up(const Mouse::Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left)
    ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  else if (b == Button::Right)
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

  SendInput(1, &ip, sizeof(ip));
}

}  // namespace StillSane::Module::Mouse
