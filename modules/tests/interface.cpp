#include <catch2/catch.hpp>

#include <module/interface/interface.hh>

namespace StillSane::Module {

TEST_CASE("interface initialization", "[interface][initialization]") {
  REQUIRE(Interface::initialize());
}

TEST_CASE("interface termination", "[interface][termination]") {
  Interface::terminate();
}

}  // namespace StillSane::Module
