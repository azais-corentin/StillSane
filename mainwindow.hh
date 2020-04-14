#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>

#include <poe/searchmanager.hh>
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

  // Slots
  void onSearchAdded();

 private:
  ::Ui::MainWindow* ui;

  Poe::SearchManager                       mSearchManager;
  Ui::Models::SearchTable                  mSearchTableModel;
  std::unique_ptr<Ui::Delegates::CheckBox> mCheckBoxDelegate;
};

}  // namespace AutoTrade

#endif  // MAINWINDOW_HH
