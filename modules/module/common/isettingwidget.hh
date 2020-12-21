#pragma once

#include <QWidget>

namespace StillSane::Module::Common {

class ISettingWidget : public QWidget {
  virtual void apply() = 0;
};

}  // namespace StillSane::Module::Common
