#pragma once

#include <compare>
#include <cstdint>

namespace StillSane::Module::Interface {

/*!
 * Positions in the interface in pixels
 */
namespace Position {
struct [[nodiscard]] Type {
  uint32_t x, y;

  friend auto operator<=>(const Type&, const Type&) = default;
  friend auto operator+(const Type& lhs, const Type& rhs)->Type;
  friend auto operator*(const Type& lhs, const double& rhs)->Type;
};

struct Type;

namespace CurrencyTab {
constexpr Type ScrollOfWisdom      = {117, 223};
constexpr Type BlacksmithsWetstone = {371, 223};
constexpr Type ArmourersScrap      = {430, 223};
constexpr Type GlassblowersBauble  = {491, 223};
constexpr Type GemcuttersPrism     = {551, 223};
constexpr Type CartographersChisel = {606, 223};
constexpr Type OrbOfTransmutation  = {056, 291};
constexpr Type OrbOfAlteration     = {116, 291};
constexpr Type OrbOfAnnulment      = {178, 291};
constexpr Type OrbOfChance         = {232, 291};
constexpr Type ExaltedOrb          = {302, 291};
constexpr Type MirrorOfKalandra    = {363, 291};
constexpr Type RegalOrb            = {430, 291};
constexpr Type OrbOfAlchemy        = {491, 291};
constexpr Type ChaosOrb            = {547, 291};
constexpr Type BlessedOrb          = {605, 291};
constexpr Type OrbOfAugmentation   = {232, 346};
constexpr Type DivineOrb           = {607, 346};
constexpr Type JewellersOrb        = {115, 420};
constexpr Type OrbOfFusing         = {175, 420};
constexpr Type ChromaticOrb        = {233, 420};
constexpr Type OrbOfScouring       = {174, 475};
constexpr Type VaalOrb             = {233, 475};
constexpr Type Item                = {333, 480};
// Aliases
constexpr Type Wisdom        = ScrollOfWisdom;
constexpr Type Wetstone      = BlacksmithsWetstone;
constexpr Type Scrap         = ArmourersScrap;
constexpr Type Bauble        = GlassblowersBauble;
constexpr Type Prism         = GemcuttersPrism;
constexpr Type Chisel        = CartographersChisel;
constexpr Type Transmutation = OrbOfTransmutation;
constexpr Type Alteration    = OrbOfAlteration;
constexpr Type Annulment     = OrbOfAnnulment;
constexpr Type Chance        = OrbOfChance;
constexpr Type Exalted       = ExaltedOrb;
constexpr Type Mirror        = MirrorOfKalandra;
constexpr Type Regal         = RegalOrb;
constexpr Type Alchemy       = OrbOfAlchemy;
constexpr Type Chaos         = ChaosOrb;
constexpr Type Blessed       = BlessedOrb;
constexpr Type Augmentation  = OrbOfAugmentation;
constexpr Type Divine        = DivineOrb;
constexpr Type Jewellers     = JewellersOrb;
constexpr Type Fusing        = OrbOfFusing;
constexpr Type Chromatic     = ChromaticOrb;
constexpr Type Scouring      = OrbOfScouring;
constexpr Type Vaal          = VaalOrb;
}  // namespace CurrencyTab

constexpr Type   InventoryFirst  = {1297, 614};
constexpr double InventoryOffset = 52.8;
Type             Inventory(const Type& position);

}  // namespace Position
using Position_t = Position::Type;

}  // namespace StillSane::Module::Interface
