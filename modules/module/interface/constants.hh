#pragma once

#include <cstdint>

namespace StillSane::Module::Interface {

/*!
 * Positions in the interface in pixels
 */
namespace Position {
struct Type {
  uint16_t x, y;
};
constexpr Type Item{333, 480};
constexpr Type Transmutation{59, 291};
constexpr Type Alteration{117, 291};
constexpr Type Annulment{175, 291};
constexpr Type Chance{233, 291};
constexpr Type Exalted{303, 291};
constexpr Type Regal{431, 291};
constexpr Type Alchemy{489, 291};
constexpr Type Chaos{547, 291};
constexpr Type Blessed{606, 291};
constexpr Type Scouring{176, 475};
constexpr Type Augmentation{233, 348};

}  // namespace Position

}  // namespace StillSane::Module::Interface
