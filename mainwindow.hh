#pragma once

#include <QMainWindow>
#include <QTimer>
#include <sol/sol.hpp>

#include <craft/crafter.hh>
#include <poe/searchmanager.hh>
#include <ui/models/searchresulttree.hh>
#include <ui/models/searchtable.hh>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace AutoTrade {

namespace Ui::Delegates {
class CheckBox;
}

class MainWindow : public QMainWindow {
  enum Hotkeys { F9 = 0x78, F10 = 0x79 };

  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // Ui slots
  void on_bAddSearch_clicked();
  void on_ePOESESSID_editingFinished();

  // Slots
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
  ::Ui::MainWindow* ui;
  QStringList       mSyntaxStyles;

  Ui::Models::SearchTable      mSearchTableModel;
  Ui::Models::SearchResultTree mSearchResultTreeModel;

  Poe::SearchManager                       mSearchManager;
  std::unique_ptr<Ui::Delegates::CheckBox> mCheckBoxDelegate;

  Craft::Crafter mCrafter;
};

}  // namespace AutoTrade
