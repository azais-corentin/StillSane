#include "module/mouse/mouse_windows.hh"

#include <windows.h>

namespace StillSane::Module {

Mouse::MouseImplementation::MouseImplementation() {
  mButtonsSwapped = GetSystemMetrics(SM_SWAPBUTTON) == TRUE;
}

Mouse::MouseImplementation::~MouseImplementation() {}

void Mouse::MouseImplementation::move(const Position_t& position) {
  INPUT ip;

  ip.type         = INPUT_MOUSE;
  ip.mi.mouseData = 0;
  ip.mi.time      = 0;
  ip.mi.dx        = position.x * (65536 / GetSystemMetrics(SM_CXSCREEN));
  ip.mi.dy        = position.y * (65536 / GetSystemMetrics(SM_CYSCREEN));
  ip.mi.dwFlags   = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &ip, sizeof(ip));
}

auto Mouse::MouseImplementation::where() -> Position_t {
  POINT p;
  GetCursorPos(&p);
  return {.x = static_cast<uint16_t>(p.x), .y = static_cast<uint16_t>(p.y)};
}

void Mouse::MouseImplementation::press(const Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left) {
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
  } else if (b == Button::Right) {
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
  }

  SendInput(1, &ip, sizeof(ip));
}

void Mouse::MouseImplementation::down(const Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left) {
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  } else if (b == Button::Right) {
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
  }

  SendInput(1, &ip, sizeof(ip));
}

void Mouse::MouseImplementation::up(const Button& b) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (b == Button::Left) {
    ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  } else if (b == Button::Right) {
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
  }

  SendInput(1, &ip, sizeof(ip));
}

auto Mouse::MouseImplementation::get_key(const Button& b) -> int {
  if (mButtonsSwapped) {
    return (b == Button::Left) ? VK_LBUTTON : VK_RBUTTON;
  }
  return (b == Button::Right) ? VK_LBUTTON : VK_RBUTTON;
}

auto Mouse::MouseImplementation::is_down(const Button& b) -> bool {
  int key = get_key(b);

  return GetAsyncKeyState(key) < 0;
}

auto Mouse::MouseImplementation::is_up(const Button& b) -> bool {
  return !is_down(b);
}

}  // namespace StillSane::Module
