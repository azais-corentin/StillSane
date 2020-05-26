#include <catch2/catch.hpp>

#include <windows.h>

#include <spdlog/spdlog.h>

#include <module/mouse/mouse.hh>

namespace StillSane::Module {

TEST_CASE("mouse initialization", "[mouse][initialization]") {
  REQUIRE(Mouse::initialize());
}

TEST_CASE("move and where", "[mouse]") {
  Mouse::Position_t pos{.x = 50, .y = 50};
  Mouse::move(pos);
  REQUIRE(Mouse::where().x == pos.x);
  REQUIRE(Mouse::where().y == pos.y);
}

TEST_CASE("mouse buttons", "[mouse]") {
  Mouse::Button b;

  SECTION("left button") { b = Mouse::Button::Left; }
  SECTION("right button") { b = Mouse::Button::Right; }

  Mouse::down(b);
  REQUIRE(!Mouse::is_up(b));
  REQUIRE(Mouse::is_down(b));

  Mouse::up(b);
  REQUIRE(Mouse::is_up(b));
  REQUIRE(!Mouse::is_down(b));

  // Ensure button b is up before leaving
  Mouse::up(b);
}

TEST_CASE("mouse termination", "[mouse][termination]") {
  Mouse::terminate();
}

}  // namespace StillSane::Module
