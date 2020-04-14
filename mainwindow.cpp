#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>

#include <network/accessmanager.hh>
#include <ui/delegates/checkbox.hh>

namespace AutoTrade {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new ::Ui::MainWindow) {
  // Setup ui
  ui->setupUi(this);
  ui->tableSearches->setModel(&mSearchTableModel);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  mCheckBoxDelegate = std::make_unique<Ui::Delegates::CheckBox>(ui->tableSearches);
  ui->tableSearches->setItemDelegateForColumn(2, mCheckBoxDelegate.get());

  // Load settings
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());

  // Connect
  connect(&mSearchManager, &Poe::SearchManager::searchAdded, this,
          &MainWindow::onSearchAdded);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_bAddSearch_clicked() {
  auto&& searchUrl  = ui->eSearchUrl->text();
  auto&& searchName = ui->eSearchName->text();

  if (!searchUrl.isEmpty() && !searchName.isEmpty()) {
    auto&& tokens = searchUrl.splitRef("/");
    auto&& id     = tokens.takeLast().toString();
    auto&& league = tokens.takeLast().toString();

    mSearchManager.addSearch(id, league, searchName);
  }
}

void MainWindow::onSearchAdded() {
  qDebug() << "MainWindow::onSearchAdded";
  mSearchTableModel.setSearches(mSearchManager.getSearches());
}

}  // namespace AutoTrade

void AutoTrade::MainWindow::on_ePOESESSID_editingFinished() {
  qDebug() << "Editing finished!";
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());
}
