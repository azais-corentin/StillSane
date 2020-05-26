find_package(spdlog CONFIG REQUIRED)

include(add_stillsane_module)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

find_package(spdlog CONFIG REQUIRED)

##### Configure modules
# Crafting


# Interface
add_stillsane_module(Interface)
target_sources(Interface PRIVATE
  module/interface/constants.hh
  module/interface/interface.hh
  module/interface/interface.cpp
)

# ItemData


# Keyboard


# Mouse
add_stillsane_module(Mouse)
target_sources(Mouse PRIVATE
  module/mouse/constants.hh
  module/mouse/mouse.hh
  module/mouse/mouse_$<IF:${WIN32},windows,linux>.cpp
)
target_link_libraries(Mouse PRIVATE spdlog::spdlog spdlog::spdlog_header_only)


# Trading
