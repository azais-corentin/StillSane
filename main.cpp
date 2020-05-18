#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char* argv[]) {
  QCoreApplication::setOrganizationName("AutoTrade");
  QCoreApplication::setApplicationName("AutoTrade");

  QApplication a(argc, argv);

  AutoTrade::MainWindow w;
  w.show();
  return a.exec();
}
