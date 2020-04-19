#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>

#include <boost/sml.hpp>
#include <sol/sol.hpp>

#include <craft/crafter.hh>
#include <craft/statemachines.hh>
#include <poe/searchmanager.hh>
#include <ui/luahighlighter.hh>
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
  enum Hotkeys { F9 = 0, F10 = 1 };

  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // Ui slots
  void on_bAddSearch_clicked();
  void on_ePOESESSID_editingFinished();
  void on_bLoadFrom_clicked();
  void on_bSaveTo_clicked();

  // Slots
  void onSearchAdded();
  void onNewResult();

 private:
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
  ::Ui::MainWindow*            ui;
  Ui::LuaHighlighter*          mLuaHighlighter;
  Ui::Models::SearchTable      mSearchTableModel;
  Ui::Models::SearchResultTree mSearchResultTreeModel;

  Poe::SearchManager                       mSearchManager;
  std::unique_ptr<Ui::Delegates::CheckBox> mCheckBoxDelegate;

  Craft::Crafter mCrafter;
};

}  // namespace AutoTrade

#endif  // MAINWINDOW_HH
