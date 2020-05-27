#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <windows.h>
#include <optional>

#include <spdlog/spdlog.h>
#include <QSettings>

#include "version.hh"

#include <module/interface/interface.hh>
#include <module/mouse/mouse.hh>

namespace StillSane {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new ::Ui::MainWindow) {
  // Logging
  spdlog::set_level(spdlog::level::trace);
  setupUi();
  initializeModules();
  setupConnections();

  loadSettings();
}

MainWindow::~MainWindow() {
  saveSettings();
  terminateModules();

  delete ui;
}

void MainWindow::setupUi() {
  ui->setupUi(this);
  setWindowTitle(QString("StillSane v") + Version);

  ui->menubar->addAction(ui->actionSettings);
}

void MainWindow::setupConnections() {
  connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::settingsDialog);
}

void MainWindow::initializeModules() {
  using namespace Module;

  Mouse::initialize();
  Interface::initialize();

  Mouse::move(Interface::Position::Item);
}

void MainWindow::terminateModules() {
  using namespace Module;

  Mouse::terminate();
  Interface::terminate();
}

auto MainWindow::nativeEvent(const QByteArray& /*eventType*/,
                             void* message,
                             long* /*result*/) -> bool {
  MSG* msg = static_cast<MSG*>(message);
  if (msg->message == WM_HOTKEY) {
    // Pass to proper module
    return true;
  }
  return false;
}

void MainWindow::saveSettings() {
  QSettings settings;
}

void MainWindow::loadSettings() {
  QSettings settings;
}

void MainWindow::settingsDialog() {
  // Ui::Dialogs::ModuleConfiguration dialog(this);

  // dialog.exec();
}

}  // namespace StillSane
