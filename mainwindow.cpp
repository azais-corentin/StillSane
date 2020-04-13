#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  // Setup ui
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}
