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

/*
//////////////////////

template <class NonMap>
struct Print {
  static void print(const QString& tabs, const NonMap& value) {
    qDebug() << tabs << value;
  }
};

template <class Key, class ValueType>
struct Print<class QMap<Key, ValueType>> {
  static void print(const QString& tabs, const QMap<Key, ValueType>& map) {
    const QString                extraTab = tabs + "    ";
    QMapIterator<Key, ValueType> iterator(map);
    while (iterator.hasNext()) {
      iterator.next();
      qDebug() << tabs << iterator.key();
      Print<ValueType>::print(extraTab, iterator.value());
    }
  }
};

//////////////////////

struct Node {
  enum Type { Root, Object, Array, Value };

  // if type != Root
  QString name;

  Type type;

  // if type == Object || type == Root
  QVector<std::shared_ptr<Node>> objects;
  // if type == Array
  QVector<QJsonValue> values;
  // if type == Value
  QJsonValue value;

  std::shared_ptr<Node> create(QString key) {
    auto node = std::make_shared<Node>();
    objects.push_back(node);
    node->name = key;
    return node;
  }
};

std::shared_ptr<Node> get_node(std::shared_ptr<Node> node, QString key) {
  if (node->type == Node::Root || node->type == Node::Object) {
    auto it =
        std::find_if(node->objects.begin(), node->objects.end(),
                     [&](const std::shared_ptr<Node>& n) { return n->name == key; });
    if (it != node->objects.end())
      return *it;
  }

  return nullptr;
}

void add_value(std::shared_ptr<Node> node, QString key, QJsonValue value) {
  auto created_node   = node->create(key);
  created_node->type  = Node::Value;
  created_node->value = value;
}

void add_value(std::shared_ptr<Node> node, QStringList keys, QJsonValue value) {
  // qDebug() << "add_value:" << keys << ":" << value;

  if (keys.size() == 1) {
    add_value(node, keys.last(), value);  // end of recursion
    return;
  }

  auto next_key  = keys.takeFirst();
  auto next_node = get_node(node, next_key);

  if (!next_node) {
    next_node       = node->create(next_key);
    next_node->type = Node::Object;
  }

  add_value(next_node, keys, value);  // recurse
}

void traverse(std::shared_ptr<Node> node, int indent = 0) {
  QString indentation;
  for (int i = indent; i > 0; i--)
    indentation += "    ";

  if (node->type == Node::Root || node->type == Node::Object) {
    qDebug() << indentation + node->name;
    for (const auto& child : node->objects)
      traverse(child, indent + 1);
  } else if (node->type == Node::Array) {
    qDebug() << indentation + node->name;
    for (const auto& value : node->values) {
      qDebug() << indentation + value.toString();
    }
  } else if (node->type == Node::Value) {
    qDebug() << indentation + node->name + "=" + node->value.toString();
  }
}

QJsonValue to_json(std::shared_ptr<Node> node) {
  QJsonValue out;
  if (node->type == Node::Root || node->type == Node::Object) {
    QJsonObject obj;
    for (const auto& child : node->objects)
      obj.insert(child->name, to_json(child));
    out = obj;
  } else if (node->type == Node::Array) {
    QJsonArray array;
    for (const auto& value : node->values) {
      array.append(value);
    }
    out = array;
  } else if (node->type == Node::Value) {
    out = node->value;  // end of recursion
  }
  return out;
}

bool readJsonFile(QIODevice& device, QSettings::SettingsMap& map) {
  QJsonParseError error;
  map = QJsonDocument::fromJson(device.readAll(), &error).toVariant().toMap();

  qDebug() << "";
  qDebug() << "";
  Print<QSettings::SettingsMap>::print("", map);
  qDebug() << "";
  qDebug() << "";

  return error.error == QJsonParseError::NoError;
}

bool writeJsonFile(QIODevice& device, const QSettings::SettingsMap& map) {
  qDebug() << "";
  qDebug() << "";
  Print<QSettings::SettingsMap>::print("", map);
  qDebug() << "";
  qDebug() << "";

  auto i = map.constBegin();

  auto root  = std::make_shared<Node>();
  root->type = Node::Root;

  while (i != map.constEnd()) {
    qDebug() << "value:" << i.value().toJsonValue();
    add_value(root, i.key().split("/"), i.value().toJsonValue());
    i++;
  }

  // traverse(root);
  QJsonDocument json;
  json.setObject(to_json(root).toObject());

  qDebug() << json.toJson();

  const auto out = json.toJson();
  return device.write(out) == out.size();
}
*/

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

  delete mLuaHighlighterFunctions;
  delete mLuaHighlighterTransitionTable;

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
  ui->dockManagerMain->setStyleSheet("");
  ui->dockManagerAutoCraft->setStyleSheet("");

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

      auto style = new QSyntaxStyle(this);
      if (!style->load(file.readAll())) {
        spdlog::error("Failed to load style {}", filepath.toStdString());
        delete style;
        return;
      }

      mStyles.append({style->name(), style});
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
  int id = QFontDatabase::addApplicationFont("resources/fonts/SourceCodePro-Regular.otf");
  if (id == -1) {
    ui->statusbar->showMessage("Unable to load font 'SourceCodePro-Regular.otf'");
    spdlog::warn("Unable to load font 'SourceCodePro-Regular.otf'");
  } else {
    spdlog::debug("Loaded font 'SourceCodePro-Regular.otf'");
    family = QFontDatabase::applicationFontFamilies(id).at(0);
  }
  QFont font(family, 10);
  font.setStyleHint(QFont::Monospace);

  ui->editorFunctions->setFont(font);
  ui->editorTransitionTable->setFont(font);

  mLuaCompleter                  = new QLuaCompleter(this);
  mLuaHighlighterFunctions       = new QLuaHighlighter;
  mLuaHighlighterTransitionTable = new QLuaHighlighter;
  ui->editorFunctions->setCompleter(mLuaCompleter);
  ui->editorTransitionTable->setCompleter(mLuaCompleter);
  ui->editorFunctions->setHighlighter(mLuaHighlighterFunctions);
  ui->editorTransitionTable->setHighlighter(mLuaHighlighterTransitionTable);

  loadSyntaxStyles();
  // Set last loaded style
  if (!mStyles.empty()) {
    ui->editorFunctions->setSyntaxStyle(mStyles.last().second);
    ui->editorTransitionTable->setSyntaxStyle(mStyles.last().second);
  }
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
  QJsonObject editors;
  QJsonObject plaintext;
  plaintext["functions"]        = ui->editorFunctions->toPlainText();
  plaintext["transition_table"] = ui->editorTransitionTable->toPlainText();
  editors["plaintext"]          = plaintext;
  settings["editors"]           = editors;

  settingsFile.write(QJsonDocument(settings).toJson());
}

void MainWindow::loadSettings() {
  QFile settingsFile("settings.json");
  if (!settingsFile.open(QIODevice::ReadOnly)) {
    // default settings
    ui->editorFunctions->clear();
    ui->editorTransitionTable->clear();
    return;
  }

  const auto& settings = QJsonDocument::fromJson(settingsFile.readAll()).object();

  const auto& editors   = settings["editors"].toObject();
  const auto& plaintext = editors["plaintext"].toObject();
  const auto& searches  = settings["searches"].toArray();

  ui->editorFunctions->setPlainText(plaintext["functions"].toString());
  ui->editorTransitionTable->setPlainText(plaintext["transition_table"].toString());

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
