#pragma once

#include <QMainWindow>
#include <QMultiMap>

#include "craft/crafter.hh"
#include "poe/searchmanager.hh"
#include "ui/models/searchresulttree.hh"
#include "ui/models/searchtable.hh"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace AutoTrade {

namespace Ui {
class LuaEditor;
namespace Delegates {
class CheckBox;
}
}  // namespace Ui

class MainWindow : public QMainWindow {
  enum Hotkeys { F9 = 0x78, F10 = 0x79 };
  enum class EditorType : int {
    Functions,
    F = Functions,
    TransitionTable,
    TT = TransitionTable
  };

  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // Ui slots
  void on_bAddSearch_clicked();
  void on_ePOESESSID_editingFinished();

  // App slots
  void onSearchAdded();
  void onNewResult();

 private:
  // Ui
  void setupUi();
  void loadSyntaxStyles();

  // Crafting
  bool nativeEvent(const QByteArray& eventType, void* message, long* result) final;
  void registerHotkeys();
  void unregisterHotkeys();
  void setupCraftingEditor();

  void startCrafting();
  void stopCrafting();

  void saveSettings();
  void loadSettings();

 private:
  ::Ui::MainWindow*                     ui;
  QStringList                           mSyntaxStyles;
  QMultiMap<EditorType, Ui::LuaEditor*> mLuaEditors;
  // QVector<Ui::LuaEditor*> mLuaEditors;

  Ui::Models::SearchTable      mSearchTableModel;
  Ui::Models::SearchResultTree mSearchResultTreeModel;

  Poe::SearchManager                       mSearchManager;
  std::unique_ptr<Ui::Delegates::CheckBox> mCheckBoxDelegate;

  Craft::Crafter mCrafter;
};

}  // namespace AutoTrade
