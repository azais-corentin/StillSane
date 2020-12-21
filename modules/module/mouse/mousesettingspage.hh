#pragma once

#include "ui_mousesettingspage.h"

class MouseSettingsPage : public QWidget {
  Q_OBJECT

 public:
  explicit MouseSettingsPage(QWidget* parent = nullptr);

 private:
  Ui::MouseSettingsPage ui;
};
