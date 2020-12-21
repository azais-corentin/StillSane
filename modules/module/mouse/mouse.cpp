#include "mouse.hh"

#include <module/mouse/platform_include.hh>

namespace StillSane::Module {

Mouse::Mouse() {
  mImpl = new MouseImplementation;
}

Mouse::~Mouse() {
  delete mImpl;
}

void Mouse::move(const Position_t& position) {
  mImpl->move(position);
}

Mouse::Position_t Mouse::where() {
  return mImpl->where();
}

void Mouse::press(const Button& b) {
  mImpl->press(b);
}

void Mouse::down(const Button& b) {
  mImpl->down(b);
}

void Mouse::up(const Button& b) {
  mImpl->up(b);
}

bool Mouse::is_down(const Button& b) {
  return mImpl->is_down(b);
}

bool Mouse::is_up(const Button& b) {
  return mImpl->is_up(b);
}

}  // namespace StillSane::Module
