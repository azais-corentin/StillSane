find_package(spdlog CONFIG REQUIRED)

include(add_stillsane_module)

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
  modules/mouse/mouse.cpp
)

# Trading
