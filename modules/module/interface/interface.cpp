#include <module/interface/interface.hh>

#include <module/interface/constants.hh>

namespace StillSane::Module::Interface {

namespace Position {

auto operator+(const Type& lhs, const Type& rhs) -> Type {
  return Type{lhs.x + rhs.x, lhs.y + rhs.y};
}

auto operator*(const Type& lhs, const double& rhs) -> Type {
  return {.x = static_cast<uint32_t>(lhs.x * rhs),
          .y = static_cast<uint32_t>(lhs.y * rhs)};
}

Type Inventory(const Type& position) {
  return InventoryFirst + position * InventoryOffset;
}

}  // namespace Position

auto initialize() -> bool {
  return true;
}

void terminate() {}

}  // namespace StillSane::Module::Interface
