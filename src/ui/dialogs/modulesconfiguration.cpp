#include "modulesconfiguration.hh"

#include <spdlog/spdlog.h>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QStackedWidget>

/*#include <module/interface/settings.hh>
#include <module/mouse/settings.hh>*/

namespace StillSane::Ui::Dialogs {

ModulesConfiguration::ModulesConfiguration(QWidget* parent) : QDialog(parent) {
  setupUi();
}

void ModulesConfiguration::setupUi() {
  mGridLayout = new QGridLayout();
  setLayout(mGridLayout);

  mModulesFilter  = new QLineEdit;
  mModuleName     = new QLabel;
  mModulesList    = new QListWidget;
  mModulesWidgets = new QStackedWidget;
  mButtons        = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel |
                                  QDialogButtonBox::Apply);

  mGridLayout->addWidget(mModulesFilter, 0, 0);
  mGridLayout->addWidget(mModuleName, 0, 1);
  mGridLayout->addWidget(mModulesList, 1, 0);
  mGridLayout->addWidget(mModulesWidgets, 1, 1);
  mGridLayout->addWidget(mButtons, 2, 0, 1, 2);

  // Filter
  mModulesFilter->setPlaceholderText("Filter");

  // List
  mModulesList->setIconSize({16, 16});
  mModulesList->setSelectionBehavior(QAbstractItemView::SelectRows);

  mGridLayout->setColumnStretch(1, 1);
  mModulesList->setSelectionMode(QAbstractItemView::SingleSelection);

  setMinimumSize(640, 360);
  setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

}  // namespace StillSane::Ui::Dialogs
