find_package(spdlog CONFIG REQUIRED)

include(add_stillsane_module)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

find_package(spdlog CONFIG REQUIRED)

##### Define modules
# Crafting


# Interface
add_stillsane_module(Interface)
target_sources(Interface PRIVATE
  modules/interface/constants.hh
  modules/interface/interface.cpp
)

# ItemData


# Keyboard


# Mouse
add_stillsane_module(Mouse)
target_sources(Mouse PRIVATE
  modules/mouse/mouse.hh
  modules/mouse/mouse_$<IF:${WIN32},windows,linux>.cpp
)
target_link_libraries(Mouse PRIVATE spdlog::spdlog spdlog::spdlog_header_only)


# Trading
