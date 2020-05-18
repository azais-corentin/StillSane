#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <windows.h>
#include <optional>

#include <spdlog/spdlog.h>
#include <QFile>
#include <QFileDialog>
#include <QLuaCompleter>
#include <QLuaHighlighter>
#include <QSyntaxStyle>
#include <range/v3/view/reverse.hpp>

#include <network/accessmanager.hh>
#include <poe/api/trade.hh>
#include <ui/delegates/checkbox.hh>

namespace AutoTrade {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new ::Ui::MainWindow) {
  // Logging
  spdlog::set_level(spdlog::level::trace);

  // Default flags for docks
  {
    using namespace ads;
    CDockManager::setConfigFlags(CDockManager::DefaultOpaqueConfig);
    CDockManager::setConfigFlag(CDockManager::ActiveTabHasCloseButton, false);
    CDockManager::setConfigFlag(CDockManager::DockAreaHasCloseButton, false);
    CDockManager::setConfigFlag(CDockManager::DockAreaHasUndockButton, false);
    CDockManager::setConfigFlag(CDockManager::DockAreaHasTabsMenuButton, false);
  }

  // auto settingsFormat = QSettings::registerFormat("json", readJsonFile, writeJsonFile);
  // QSettings::setDefaultFormat(settingsFormat);

  // User interface
  setupUi();

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
  spdlog::debug("MainWindow::onSearchAdded");
  mSearchTableModel.setSearches(mSearchManager.getSearches());
}

void MainWindow::onNewResult() {
  spdlog::debug("MainWindow::onNewResult");
}

void MainWindow::setupUi() {
  using namespace ads;

  ui->setupUi(this);

  // Setup main dock widgets
  CDockWidget* dockWidgetTabOngoingTrades = new CDockWidget("Ongoing trades");
  dockWidgetTabOngoingTrades->setWidget(ui->tabOngoingTrades);
  CDockWidget* dockWidgetTabFinishedTrades = new CDockWidget("Finished trades");
  dockWidgetTabFinishedTrades->setWidget(ui->tabFinishedTrades);
  CDockWidget* dockWidgetTabSearchResults = new CDockWidget("Search results");
  dockWidgetTabSearchResults->setWidget(ui->tabSearchResults);
  CDockWidget* dockWidgetTabSearches = new CDockWidget("Searches");
  dockWidgetTabSearches->setWidget(ui->tabSearches);
  CDockWidget* dockWidgetTabSettings = new CDockWidget("Settings");
  dockWidgetTabSettings->setWidget(ui->tabSettings);
  CDockWidget* dockWidgetTabAutoCraft = new CDockWidget("Auto craft");
  dockWidgetTabAutoCraft->setWidget(ui->tabAutoCraft);
  // Add them to dockManagerMain
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabOngoingTrades);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabFinishedTrades);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabSearchResults);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabSearches);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabSettings);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, dockWidgetTabAutoCraft);
  ui->mainTabWidget->deleteLater();  // Remove because it was only used in the designer

  // Setup AutoCraft dock widgets
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
  // Add them to dockManagerAutoCraft
  // Add the dock widget to the top dock widget area
  ui->dockManagerAutoCraft->addDockWidget(ads::LeftDockWidgetArea,
                                          dockWidgetEditorFunctions);
  ui->dockManagerAutoCraft->addDockWidget(ads::RightDockWidgetArea,
                                          dockWidgetEditorTransitionTable);

  // Searches table
  ui->tableSearches->setModel(&mSearchTableModel);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  mCheckBoxDelegate = std::make_unique<Ui::Delegates::CheckBox>(ui->tableSearches);
  ui->tableSearches->setItemDelegateForColumn(2, mCheckBoxDelegate.get());
  // Setup search results tree
  ui->treeSearchResults->setModel(&mSearchResultTreeModel);
}

void MainWindow::loadSyntaxStyles() {
  auto current = QDir::current();
  if (current.cd("resources/styles")) {
    for (auto filepath :
         current.entryList(QStringList() << "*.xml", QDir::Filter::Files)) {
      QFile file(current.absoluteFilePath(filepath));
      if (!file.open(QIODevice::ReadOnly)) {
        spdlog::error("Failed to open file {}", filepath.toStdString());
        return;
      }

      QString contents = file.readAll();

      ui->eEditorFunctions->addSyntaxStyle(contents);
      QString name = ui->eEditorTransitionTable->addSyntaxStyle(contents);

      if (!name.isEmpty())
        mSyntaxStyles.append(name);
    }
  }
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
  int     fontId =
      QFontDatabase::addApplicationFont("resources/fonts/SourceCodePro-Regular.otf");
  if (fontId == -1) {
    ui->statusbar->showMessage("Unable to load font 'SourceCodePro-Regular.otf'");
    spdlog::warn("Unable to load font 'SourceCodePro-Regular.otf'");
  } else {
    spdlog::debug("Loaded font 'SourceCodePro-Regular.otf'");
    family = QFontDatabase::applicationFontFamilies(fontId).at(0);
  }
  QFont font(family, 10);
  font.setStyleHint(QFont::Monospace);

  ui->eEditorFunctions->setFont(font);
  ui->eEditorTransitionTable->setFont(font);

  loadSyntaxStyles();
  // Set last loaded style
  if (!mSyntaxStyles.empty()) {
    ui->eEditorFunctions->setSyntaxStyle(mSyntaxStyles.last());
    ui->eEditorTransitionTable->setSyntaxStyle(mSyntaxStyles.last());
  }

  ui->eEditorFunctions->setDefaultPath("resources/functions");
  ui->eEditorTransitionTable->setDefaultPath("resources/transition_tables");
}

void MainWindow::startCrafting() {
  auto codeTransitionTable = ui->eEditorTransitionTable->text();
  auto codeFunctions       = ui->eEditorFunctions->text();

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
  QJsonObject editors;
  QJsonObject plaintext;
  plaintext["functions"]        = ui->eEditorFunctions->text();
  plaintext["transition_table"] = ui->eEditorTransitionTable->text();
  editors["plaintext"]          = plaintext;
  settings["editors"]           = editors;

  settingsFile.write(QJsonDocument(settings).toJson());
}

void MainWindow::loadSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::ReadOnly)) {
    // default settings
    ui->eEditorFunctions->clear();
    ui->eEditorTransitionTable->clear();
    return;
  }

  const auto& settings = QJsonDocument::fromJson(settingsFile.readAll()).object();

  const auto& editors   = settings["editors"].toObject();
  const auto& plaintext = editors["plaintext"].toObject();
  const auto& searches  = settings["searches"].toArray();

  ui->eEditorFunctions->setText(plaintext["functions"].toString());
  ui->eEditorTransitionTable->setText(plaintext["transition_table"].toString());

  for (const auto& search : searches) {
    mSearchManager.addSearch(search["id"].toString(), search["league"].toString(),
                             search["name"].toString(), search["enabled"].toBool());
  }
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
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());
}

}  // namespace AutoTrade
