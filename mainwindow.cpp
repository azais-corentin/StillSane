#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <windows.h>

#include <QDebug>
#include <QFile>
#include <QFileDialog>

#include <network/accessmanager.hh>
#include <poe/api/trade.hh>
#include <ui/delegates/checkbox.hh>

namespace AutoTrade {

constexpr char const* defaultLuaScript = "";

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new ::Ui::MainWindow) {
  using namespace ads;
  CDockManager::setConfigFlags(CDockManager::DefaultOpaqueConfig);
  CDockManager::setConfigFlag(CDockManager::ActiveTabHasCloseButton, false);
  CDockManager::setConfigFlag(CDockManager::DockAreaHasCloseButton, false);
  CDockManager::setConfigFlag(CDockManager::DockAreaHasUndockButton, false);
  CDockManager::setConfigFlag(CDockManager::DockAreaHasTabsMenuButton, false);
  // Setup ui
  ui->setupUi(this);

  CDockWidget* dockWidgetEditorTransitionTable = new CDockWidget("State machine editor");
  CDockWidget* dockWidgetEditorFunctions       = new CDockWidget("Functions editor");
  dockWidgetEditorFunctions->setWidget(ui->widgetFunctions,
                                       CDockWidget::ForceNoScrollArea);
  dockWidgetEditorFunctions->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromContent);
  dockWidgetEditorTransitionTable->setWidget(ui->widgetTransitionTable,
                                             CDockWidget::ForceNoScrollArea);
  dockWidgetEditorTransitionTable->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromContent);

  // Add the dock widget to the top dock widget area
  ui->dockWidget->addDockWidget(ads::LeftDockWidgetArea, dockWidgetEditorFunctions);
  ui->dockWidget->addDockWidget(ads::RightDockWidgetArea,
                                dockWidgetEditorTransitionTable);

  // Setup search table
  ui->tableSearches->setModel(&mSearchTableModel);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  mCheckBoxDelegate = std::make_unique<Ui::Delegates::CheckBox>(ui->tableSearches);
  ui->tableSearches->setItemDelegateForColumn(2, mCheckBoxDelegate.get());
  // Setup search results tree
  ui->treeSearchResults->setModel(&mSearchResultTreeModel);

  registerHotkeys();
  setupCraftingEditor();

  // Connect
  connect(&mSearchManager, &Poe::SearchManager::searchAdded, this,
          &MainWindow::onSearchAdded);
  connect(&mSearchManager, &Poe::SearchManager::newResult, this,
          &MainWindow::onNewResult);

  connect(&mCrafter, &Craft::Crafter::info,
          [&](const QString& message) { ui->statusbar->showMessage(message); });
  connect(&mCrafter, &Craft::Crafter::error,
          [&](const QString& message) { ui->statusbar->showMessage(message); });

  // Load settings
  loadSettings();
  // \todo Is this needed? If the signal are properly connected, this shouldn't be
  // necessary. It will be updated after the settings are loaded.
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());
}

MainWindow::~MainWindow() {
  unregisterHotkeys();
  saveSettings();

  delete ui;
}

void MainWindow::onSearchAdded() {
  qDebug() << "MainWindow::onSearchAdded";
  mSearchTableModel.setSearches(mSearchManager.getSearches());
}

void MainWindow::onNewResult() {
  qDebug() << "New result!";
}

bool MainWindow::nativeEvent(const QByteArray&, void* message, long*) {
  MSG* msg = static_cast<MSG*>(message);
  if (msg->message == WM_HOTKEY) {
    HWND activeWindow = GetForegroundWindow();
    if (activeWindow) {
      char winTitle[256];
      int  length = GetWindowTextA(activeWindow, winTitle, 256);
      if (QString::fromLatin1(winTitle, length) == "Path of Exile") {
        switch (msg->wParam) {
          case F9:
            startCrafting();
            break;
          case F10:
            stopCrafting();
            break;
        }
      } else {
        ui->statusbar->showMessage("Start/stop only works inside the game");
      }
    }

    return true;
  }
  return false;
}

void MainWindow::registerHotkeys() {
  RegisterHotKey(HWND(winId()), F9, 0, F9);
  RegisterHotKey(HWND(winId()), F10, 0, F10);
}

void MainWindow::unregisterHotkeys() {
  UnregisterHotKey(HWND(winId()), F9);
  UnregisterHotKey(HWND(winId()), F10);
}

void MainWindow::setupCraftingEditor() {
  QString family = "Source Code Pro";
  int id = QFontDatabase::addApplicationFont("resources/fonts/SourceCodePro-Regular.otf");
  if (id == -1) {
    ui->statusbar->showMessage("Unable to load font 'SourceCodePro-Regular.otf'");
    qWarning() << "Unable to load font 'SourceCodePro-Regular.otf'";
  } else {
    qDebug() << "Loaded font 'SourceCodePro-Regular.otf'";
    family = QFontDatabase::applicationFontFamilies(id).at(0);
  }
  QFont font(family, 10);
  font.setStyleHint(QFont::Monospace);

  ui->editorFunctions->setFont(font);
  ui->editorTransitionTable->setFont(font);

  mLuaHighlighterFunctions = new Ui::LuaHighlighter(ui->editorFunctions->document());
  mLuaHighlighterTransitionTable =
      new Ui::LuaHighlighter(ui->editorTransitionTable->document());
}

void MainWindow::startCrafting() {
  auto codeTransitionTable = ui->editorTransitionTable->toPlainText();
  auto codeFunctions       = ui->editorFunctions->toPlainText();

  mCrafter.start(codeTransitionTable.toStdString(), codeFunctions.toStdString());
}

void MainWindow::stopCrafting() {
  mCrafter.stop();
}

void MainWindow::saveSettings() {
  QFile settingsFile("settings.json");
  settingsFile.open(QIODevice::WriteOnly);
  QJsonObject settings;

  QJsonArray  searchesArray;
  const auto& searches = mSearchManager.getSearches();
  for (const auto& search : searches) {
    searchesArray.append(QJsonObject{{"id", search->getId()},
                                     {"league", search->getLeague()},
                                     {"name", search->getName()},
                                     {"enabled", search->isEnabled()}});
  }

  settings["searches"] = searchesArray;
  settings["lua"] =
      QJsonObject({{"functions", ui->editorFunctions->toPlainText()},
                   {"transition_table", ui->editorTransitionTable->toPlainText()}});

  settingsFile.write(QJsonDocument(settings).toJson());
}

void MainWindow::loadSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::ReadOnly)) {
    // default settings
    ui->editorFunctions->setPlainText(defaultLuaScript);
    ui->editorTransitionTable->setPlainText(defaultLuaScript);
    return;
  }
  const auto& settings = QJsonDocument::fromJson(settingsFile.readAll()).object();

  const auto& searches = settings["searches"].toArray();
  for (const auto& search : searches) {
    mSearchManager.addSearch(search["id"].toString(), search["league"].toString(),
                             search["name"].toString(), search["enabled"].toBool(true));
  }

  ui->editorFunctions->setPlainText(
      settings["lua"]["functions"].toString(defaultLuaScript));
  ui->editorTransitionTable->setPlainText(
      settings["lua"]["transition_table"].toString(defaultLuaScript));
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

void MainWindow::on_ePOESESSID_editingFinished() {
  qDebug() << "Editing finished!";
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());
}

void MainWindow::on_bLoadFunctions_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open functions"), "resources/functions", tr("Lua script (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::ReadOnly)) {
    ui->editorFunctions->setPlainText(lua.readAll());
  } else {
    ui->statusbar->showMessage("Could not open file:" + fileName);
  }
}

void MainWindow::on_bSaveFunctions_clicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save functions"), "resources/functions", tr("Lua script (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::WriteOnly)) {
    lua.write(ui->editorFunctions->toPlainText().toUtf8());
  } else {
    ui->statusbar->showMessage("Could not write to file:" + fileName);
  }
}

void MainWindow::on_bLoadTransitionTable_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open transition table"),
                                                  "resources/transition_tables",
                                                  tr("Lua script (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::ReadOnly)) {
    ui->editorTransitionTable->setPlainText(lua.readAll());
  } else {
    ui->statusbar->showMessage("Could not open file:" + fileName);
  }
}

void MainWindow::on_bSaveTransitionTable_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save transition table"),
                                                  "resources/transition_tables",
                                                  tr("Lua script (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::WriteOnly)) {
    lua.write(ui->editorTransitionTable->toPlainText().toUtf8());
  } else {
    ui->statusbar->showMessage("Could not write to file:" + fileName);
  }
}

}  // namespace AutoTrade
