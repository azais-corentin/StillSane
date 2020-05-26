#include "module/mouse/mouse.hh"

#include <windows.h>

#include <spdlog/spdlog.h>

/**
 * Windows implementation of the mouse module
 */
namespace StillSane::Module::Mouse {

inline bool buttonsSwapped;

bool initialize() {
  buttonsSwapped = GetSystemMetrics(SM_SWAPBUTTON) == TRUE;
  spdlog::info("swapped: {}", buttonsSwapped);
  return true;
}

void terminate() {}

void move(const Position_t& position) {
  INPUT ip;

  ip.type         = INPUT_MOUSE;
  ip.mi.mouseData = 0;
  ip.mi.time      = 0;
  ip.mi.dx = static_cast<LONG>(1 + position.x * (65536. / GetSystemMetrics(SM_CXSCREEN)));
  ip.mi.dy = static_cast<LONG>(1 + position.y * (65536. / GetSystemMetrics(SM_CYSCREEN)));
  ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &ip, sizeof(ip));
}

Position_t where() {
  POINT p;
  GetCursorPos(&p);
  return {.x = static_cast<uint16_t>(p.x), .y = static_cast<uint16_t>(p.y)};
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

inline int get_key(const Button& b) {
  if (buttonsSwapped)
    return (b == Button::Left) ? VK_LBUTTON : VK_RBUTTON;
  else
    return (b == Button::Right) ? VK_LBUTTON : VK_RBUTTON;
}

bool is_down(const Button& b) {
  int key = get_key(b);

  return GetAsyncKeyState(key) < 0;
}

bool is_up(const Button& b) {
  return !is_down(b);
}

}  // namespace StillSane::Module::Mouse
