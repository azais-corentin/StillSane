#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>

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
    auto&& id     = tokens.takeLast();
    auto&& league = tokens.takeLast();

    QString searchId = league + "/" + id;

    mSearchManager.addSearch(searchId, searchName);
  }
}

void MainWindow::onSearchAdded() {
  qDebug() << "MainWindow::onSearchAdded";
  mSearchTableModel.setSearches(mSearchManager.getSearches());
}

}  // namespace AutoTrade
