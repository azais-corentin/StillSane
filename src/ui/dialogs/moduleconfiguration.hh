#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QVBoxLayout;
class QTableView;
class QDialogButtonBox;
QT_END_NAMESPACE

namespace StillSane {

class MainWindow;

namespace Ui {
namespace Models {
class ModulesTable;
}

namespace Dialogs {

class ModuleConfiguration : public QDialog {
  Q_OBJECT
 public:
  explicit ModuleConfiguration(MainWindow* mainWindow);
  ~ModuleConfiguration();

 private:
  MainWindow* mMainWindow;
  // Layouts
  QVBoxLayout* mMainLayout;
  QHBoxLayout* mButtonsLayout;
  // Widgets
  QTableView*       mModulesTable;
  QDialogButtonBox* mButtonBox;
  // Models
  Models::ModulesTable* mModulesTableModel;
};

}  // namespace Dialogs
}  // namespace Ui
}  // namespace StillSane
