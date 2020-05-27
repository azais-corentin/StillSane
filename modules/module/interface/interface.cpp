#include <module/interface/interface.hh>

namespace StillSane::Module::Interface {

auto initialize() -> bool {
  return true;
}

void terminate() {}

}  // namespace StillSane::Module::Interface
