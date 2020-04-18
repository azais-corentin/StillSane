#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>

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
  void saveSettings();
  void loadSettings();

 private:
  ::Ui::MainWindow*            ui;
  Ui::Models::SearchTable      mSearchTableModel;
  Ui::Models::SearchResultTree mSearchResultTreeModel;

  Poe::SearchManager                       mSearchManager;
  std::unique_ptr<Ui::Delegates::CheckBox> mCheckBoxDelegate;
};

}  // namespace AutoTrade

#endif  // MAINWINDOW_HH
