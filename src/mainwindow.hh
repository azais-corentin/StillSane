#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace StillSane {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private:
  // Setup
  void setupUi();
  void setupConnections();
  void initializeModules();

  // Cleanup
  void terminateModules();

  void configureModules();

  // Settings
  void saveSettings();
  void loadSettings();

  // Hotkeys
  bool nativeEvent(const QByteArray& eventType, void* message, long* result) final;

 private:
  ::Ui::MainWindow* ui;
};

}  // namespace StillSane
