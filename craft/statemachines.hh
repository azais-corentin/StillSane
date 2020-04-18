#ifndef AUTOTRADE_CRAFT_STATEMACHINES_HH
#define AUTOTRADE_CRAFT_STATEMACHINES_HH

#include <boost/sml.hpp>

#include <craft/crafter.hh>

namespace AutoTrade::Craft::StateMachines {

// Guards
constexpr auto is_normal   = [](Crafter& c) { return c.is_normal(); };
constexpr auto is_magic    = [](Crafter& c) { return c.is_magic(); };
constexpr auto is_rare     = [](Crafter& c) { return c.is_rare(); };
constexpr auto is_matching = [](Crafter& c) { return c.is_matching(); };

// Actions
constexpr auto copy          = [](Crafter& c) { c.copy(); };
constexpr auto parse         = [](Crafter& c) { c.parse(); };
constexpr auto transmutation = [](Crafter& c) { c.transmutation(); };
constexpr auto alteration    = [](Crafter& c) { c.alteration(); };
constexpr auto regal         = [](Crafter& c) { c.regal(); };
constexpr auto scouring      = [](Crafter& c) { c.scouring(); };

/*!
 * \brief Represents the state machine to craft with Alteration Orbs only.
 */
struct AlterationOnly {
  auto operator()() const {
    using namespace boost::sml;
    // src_state + event [ guard ] / action = dst_state
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

}  // namespace AutoTrade::Craft::StateMachines

#endif  // AUTOTRADE_CRAFT_STATEMACHINES_HH
