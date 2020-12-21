find_package(spdlog CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include(add_stillsane_module)
include(platform_specific_include)

find_package(spdlog CONFIG REQUIRED)

##### Platform specific suffix
set(STILLSANE_PLATFORM_SUFFIX $<IF:${WIN32},windows,linux>)

##### Common module library
add_stillsane_module(Common)
target_sources(Common PRIVATE
  module/common/imodule.hh
  module/common/isettingwidget.hh
)

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
platform_specific_include(
  "module/mouse/mouse_" ".hh"
  "module/mouse/platform_include.hh"
)
target_sources(Mouse PRIVATE
  module/mouse/mouse.hh
  module/mouse/mouse.cpp
  module/mouse/mouse_${STILLSANE_PLATFORM_SUFFIX}.hh
  module/mouse/mouse_${STILLSANE_PLATFORM_SUFFIX}.cpp

  module/mouse/mousesettingspage.hh
  module/mouse/mousesettingspage.cpp
  module/mouse/mousesettingspage.ui
)
target_link_libraries(Mouse PRIVATE Qt5::Widgets)
target_link_libraries(Mouse PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

# Trading

##### Configure tests
add_subdirectory(tests)
