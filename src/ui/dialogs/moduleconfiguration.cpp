#include "moduleconfiguration.hh"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QTableView>
#include <QVBoxLayout>

#include "../../mainwindow.hh"
#include "../models/modulestable.hh"

namespace StillSane::Ui::Dialogs {

ModuleConfiguration::ModuleConfiguration(MainWindow* mainWindow)
    : QDialog(mainWindow),
      mMainWindow(mainWindow),
      mMainLayout(new QVBoxLayout()),
      mButtonsLayout(new QHBoxLayout),
      mModulesTable(new QTableView),
      mButtonBox(new QDialogButtonBox(QDialogButtonBox::Ok)),
      mModulesTableModel(new Models::ModulesTable(mainWindow, this)) {
  setWindowTitle("Modules configuration");
  setMinimumSize(640, 360);

  mModulesTable->setModel(mModulesTableModel);

  mMainLayout->addWidget(mModulesTable);
  mMainLayout->addWidget(mButtonBox);
  setLayout(mMainLayout);

  connect(mButtonBox, &QDialogButtonBox::accepted, this, &ModuleConfiguration::accept);
}

ModuleConfiguration::~ModuleConfiguration() {}

}  // namespace StillSane::Ui::Dialogs
