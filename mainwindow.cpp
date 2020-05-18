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

  /// Setup main dock widgets
  CDockWidget* wdTabOngoingTrades  = new CDockWidget("Ongoing trades");
  CDockWidget* wdTabFinishedTrades = new CDockWidget("Finished trades");
  CDockWidget* wdTabSearchResults  = new CDockWidget("Search results");
  CDockWidget* wdTabSearches       = new CDockWidget("Searches");
  CDockWidget* wdTabSettings       = new CDockWidget("Settings");
  CDockWidget* wdTabAutoCraft      = new CDockWidget("Auto craft");
  CDockWidget* wdTabAutoTrade      = new CDockWidget("Auto trade");
  wdTabOngoingTrades->setWidget(ui->tabOngoingTrades);
  wdTabFinishedTrades->setWidget(ui->tabFinishedTrades);
  wdTabSearchResults->setWidget(ui->tabSearchResults);
  wdTabSearches->setWidget(ui->tabSearches);
  wdTabSettings->setWidget(ui->tabSettings);
  wdTabAutoCraft->setWidget(ui->tabAutoCraft);
  wdTabAutoTrade->setWidget(ui->tabAutoTrade);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabOngoingTrades);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabFinishedTrades);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabSearchResults);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabSearches);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabSettings);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabAutoCraft);
  ui->dockManagerMain->addDockWidgetTab(TopDockWidgetArea, wdTabAutoTrade);
  ui->mainTabWidget->deleteLater();  // Remove because it was only used in the designer

  /// Setup AutoCraft dock widgets
  CDockWidget* wdACFEditor  = new CDockWidget("Functions editor");
  CDockWidget* wdACTTEditor = new CDockWidget("Transition table editor");
  wdACFEditor->setWidget(ui->eACFEditor, CDockWidget::ForceNoScrollArea);
  wdACFEditor->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
  wdACTTEditor->setWidget(ui->eACTTEditor, CDockWidget::ForceNoScrollArea);
  wdACTTEditor->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
  ui->dockManagerAC->addDockWidget(ads::LeftDockWidgetArea, wdACFEditor);
  ui->dockManagerAC->addDockWidget(ads::RightDockWidgetArea, wdACTTEditor);

  /// Setup AutoTrade dock widgets
  CDockWidget* wdATFEditor  = new CDockWidget("Functions editor");
  CDockWidget* wdATTTEditor = new CDockWidget("Transition table editor");
  wdATFEditor->setWidget(ui->eATFEditor, CDockWidget::ForceNoScrollArea);
  wdATFEditor->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
  wdATTTEditor->setWidget(ui->eATTTEditor, CDockWidget::ForceNoScrollArea);
  wdATTTEditor->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromContent);
  ui->dockManagerAT->addDockWidget(ads::LeftDockWidgetArea, wdATFEditor);
  ui->dockManagerAT->addDockWidget(ads::RightDockWidgetArea, wdATTTEditor);

  /// Lua editors names
  ui->eACFEditor->setName("AutoCraftFunctions");
  ui->eACTTEditor->setName("AutoCraftTransitionTable");
  ui->eATFEditor->setName("AutoTradeFunctions");
  ui->eATTTEditor->setName("AutoTradeTransitionTable");

  /// List of lua editors and their types
  mLuaEditors.insert(EditorType::F, ui->eACFEditor);
  mLuaEditors.insert(EditorType::TT, ui->eACTTEditor);
  mLuaEditors.insert(EditorType::F, ui->eATFEditor);
  mLuaEditors.insert(EditorType::TT, ui->eATTTEditor);

  /// Searches table
  ui->tableSearches->setModel(&mSearchTableModel);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  mCheckBoxDelegate = std::make_unique<Ui::Delegates::CheckBox>(ui->tableSearches);
  ui->tableSearches->setItemDelegateForColumn(2, mCheckBoxDelegate.get());

  /// Search results tree
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
      } else {
        QString contents = file.readAll();

        for (auto luaEditor : mLuaEditors) {
          /// \todo Each editor is parsing the syntax style separately. Have it parsed
          /// only once.
          QString name = luaEditor->addSyntaxStyle(contents);
          mSyntaxStyles.append(name);
          // luaEditor->setSyntaxStyle(name);
        }
      }
    }
  }

  mSyntaxStyles.sort();
  mSyntaxStyles.removeDuplicates();
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

  for (auto luaEditor : mLuaEditors)
    luaEditor->setFont(font);

  loadSyntaxStyles();
  // Set last loaded style
  if (!mSyntaxStyles.empty()) {
    for (auto luaEditor : mLuaEditors)
      luaEditor->setSyntaxStyle(mSyntaxStyles.last());
  }

  for (auto functionsEditor : mLuaEditors.values(EditorType::F))
    functionsEditor->setDefaultPath("resources/functions");
  for (auto transitionTableEditor : mLuaEditors.values(EditorType::TT))
    transitionTableEditor->setDefaultPath("resources/transition_tables");
}

void MainWindow::startCrafting() {
  auto codeFunctions       = ui->eACFEditor->text();
  auto codeTransitionTable = ui->eACTTEditor->text();

  mCrafter.start(codeTransitionTable.toStdString(), codeFunctions.toStdString());
}

void MainWindow::stopCrafting() {
  mCrafter.stop();
}

void MainWindow::saveSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::WriteOnly)) {
  }
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
  QJsonObject syntaxStyle;

  for (auto luaEditor : mLuaEditors) {
    plaintext.insert(luaEditor->name(), luaEditor->text());
    syntaxStyle.insert(luaEditor->name(), luaEditor->syntaxStyle());
  }

  editors["plaintext"]   = plaintext;
  editors["syntaxStyle"] = syntaxStyle;
  settings["editors"]    = editors;

  settingsFile.write(QJsonDocument(settings).toJson());
}

void MainWindow::loadSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::ReadOnly)) {
    // default settings
    for (auto editor : mLuaEditors)
      editor->setText("");
    return;
  }

  const auto& settings = QJsonDocument::fromJson(settingsFile.readAll()).object();

  const auto& editors     = settings["editors"].toObject();
  const auto& plaintext   = editors["plaintext"].toObject();
  const auto& syntaxStyle = editors["syntaxStyle"].toObject();
  const auto& searches    = settings["searches"].toArray();

  for (auto luaEditor : mLuaEditors) {
    luaEditor->setText(plaintext[luaEditor->name()].toString());
    luaEditor->setSyntaxStyle(syntaxStyle[luaEditor->name()].toString());
  }

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
