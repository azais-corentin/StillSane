#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <windows.h>

#include <QDebug>
#include <QFile>

#include <network/accessmanager.hh>
#include <poe/api/trade.hh>
#include <ui/delegates/checkbox.hh>

namespace AutoTrade {

namespace sml = boost::sml;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new ::Ui::MainWindow) {
  // Setup ui
  ui->setupUi(this);
  // Setup search table
  ui->tableSearches->setModel(&mSearchTableModel);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableSearches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
  mCheckBoxDelegate = std::make_unique<Ui::Delegates::CheckBox>(ui->tableSearches);
  ui->tableSearches->setItemDelegateForColumn(2, mCheckBoxDelegate.get());
  // Setup search results tree
  ui->treeSearchResults->setModel(&mSearchResultTreeModel);

  // Setup crafting
  setupCraftingEditor();

  // Connect
  connect(&mSearchManager, &Poe::SearchManager::searchAdded, this,
          &MainWindow::onSearchAdded);
  connect(&mSearchManager, &Poe::SearchManager::newResult, this,
          &MainWindow::onNewResult);

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

void MainWindow::onNewResult() {
  qDebug() << "New result!";
}

void MainWindow::on_bReload_clicked() {}

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
      }
    }

    return true;
  }
  return false;
}

void MainWindow::registerHotkeys() {
  RegisterHotKey(HWND(winId()), F9, 0, 0x78);
  RegisterHotKey(HWND(winId()), F10, 0, 0x79);
}

void MainWindow::unregisterHotkeys() {
  UnregisterHotKey(HWND(winId()), F9);
  UnregisterHotKey(HWND(winId()), F10);
}

void MainWindow::setupCraftingEditor() {
  QFont font("Source Code Pro", 10);
  font.setFixedPitch(true);
  font.setLetterSpacing(QFont::SpacingType::PercentageSpacing, 100);
  font.setWordSpacing(1);

  ui->eLua->setFont(font);

  mLuaHighlighter = new Ui::LuaHighlighter(ui->eLua->document());
}

void MainWindow::startCrafting() {
  mCrafter.start(ui->eLua->toPlainText());
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

  settings["searches"]  = searchesArray;
  settings["luascript"] = ui->eLua->toPlainText();

  settingsFile.write(QJsonDocument(settings).toJson());
}

void MainWindow::loadSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::ReadOnly))
    return;
  const auto& settings = QJsonDocument::fromJson(settingsFile.readAll()).object();

  const auto& searches = settings["searches"].toArray();
  for (const auto& search : searches) {
    mSearchManager.addSearch(search["id"].toString(), search["league"].toString(),
                             search["name"].toString(), search["enabled"].toBool(true));
  }

  ui->eLua->setPlainText(settings["luascript"].toString());
}

}  // namespace AutoTrade

void AutoTrade::MainWindow::on_ePOESESSID_editingFinished() {
  qDebug() << "Editing finished!";
  Network::AccessManager::setPOESESSID(ui->ePOESESSID->text());
}
