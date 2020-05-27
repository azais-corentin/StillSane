#pragma once

#include <QDialog>

class QGridLayout;
class QLineEdit;
class QLabel;
class QListWidget;
class QStackedWidget;
class QDialogButtonBox;

namespace StillSane::Ui::Dialogs {
class ModulesConfiguration : public QDialog {
  Q_OBJECT
 public:
  ModulesConfiguration(QWidget* parent = nullptr);

 private:
  void setupUi();

 private:
  QGridLayout*      mGridLayout;
  QLineEdit*        mModulesFilter;
  QLabel*           mModuleName;
  QListWidget*      mModulesList;
  QStackedWidget*   mModulesWidgets;
  QDialogButtonBox* mButtons;
};

}  // namespace StillSane::Ui::Dialogs
