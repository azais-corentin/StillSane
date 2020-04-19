#ifndef AUTOTRADE_CRAFT_STATEMACHINES_HH
#define AUTOTRADE_CRAFT_STATEMACHINES_HH

#include <boost/sml.hpp>

#include <craft/crafter.hh>

namespace AutoTrade::Craft::StateMachines {

/// Guards
constexpr auto is_normal   = [](Crafter& c) { return c.is_normal(); };
constexpr auto is_magic    = [](Crafter& c) { return c.is_magic(); };
constexpr auto is_rare     = [](Crafter& c) { return c.is_rare(); };
constexpr auto is_matching = [](Crafter& c) { return c.is_matching(); };

/// Actions
constexpr auto copy  = [](Crafter& c) { c.copy(); };
constexpr auto parse = [](Crafter& c) { c.parse(); };
// Orbs
constexpr auto transmutation = [](Crafter& c) { c.transmutation(); };
constexpr auto alteration    = [](Crafter& c) { c.alteration(); };
constexpr auto annulment     = [](Crafter& c) { c.annulment(); };
constexpr auto chance        = [](Crafter& c) { c.chance(); };
constexpr auto exalted       = [](Crafter& c) { c.exalted(); };
constexpr auto regal         = [](Crafter& c) { c.regal(); };
constexpr auto alchemy       = [](Crafter& c) { c.alchemy(); };
constexpr auto chaos         = [](Crafter& c) { c.chaos(); };
// constexpr auto blessed       = [](Crafter& c) { c.blessed(); };
constexpr auto scouring = [](Crafter& c) { c.scouring(); };

/*!
 * \brief Represents the state machine to craft with Orbs of Alteration only.
 *
 * \note src_state + event [ guard ] / action = dst_state
 */
struct AlterationOnly {
  auto operator()() const {
    using namespace boost::sml;
    // clang-format off
    return make_transition_table(
       *"parse"_s  + "next"_e                  / (copy, parse) = "parsed"_s,
        "parsed"_s + "next"_e [ is_normal ]    / transmutation = "parse"_s,
        "parsed"_s + "next"_e [ is_magic ]                     = "magic"_s,
        "parsed"_s + "next"_e [ is_rare ]                      = "rare"_s,

        "magic"_s  + "next"_e [ !is_matching ] / alteration    = "parse"_s,
        "magic"_s  + "next"_e [ is_matching ]  / regal         = X,
        "rare"_s   + "next"_e [ !is_matching ] / scouring      = "parse"_s,
        "rare"_s   + "next"_e [ is_matching ]                  = X
    );
    // clang-format on
  }
};

/*!
 * \brief Represents the state machine to craft with Orbs of Alchemy and Orbs of Scouring.
 *
 * \note src_state + event [ guard ] / action = dst_state
 */
struct ScouringAlchemy {
  auto operator()() const {
    using namespace boost::sml;
    // clang-format off
    return make_transition_table(
       *"parse"_s  + "next"_e                  / (copy, parse) = "parsed"_s,
        "parsed"_s + "next"_e [ is_normal ]                    = "normal"_s,
        "parsed"_s + "next"_e [ is_magic ]                     = "magic"_s,
        "parsed"_s + "next"_e [ is_rare ]                      = "rare"_s,

        "normal"_s + "next"_e                  / alchemy       = "parse"_s,
        "magic"_s  + "next"_e                  / regal         = "parse"_s,
        "rare"_s   + "next"_e [ !is_matching ] / scouring      = "parse"_s,
        "rare"_s   + "next"_e [ is_matching ]                  = X);
    // clang-format on
  }
};

}  // namespace AutoTrade::Craft::StateMachines

#endif  // AUTOTRADE_CRAFT_STATEMACHINES_HH
