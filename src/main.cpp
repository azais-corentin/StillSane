#include "mainwindow.hh"

#include <QApplication>

auto main(int argc, char* argv[]) -> int {
  QCoreApplication::setOrganizationName("StillSane");
  QCoreApplication::setApplicationName("StillSane");

  QApplication a(argc, argv);

  StillSane::MainWindow w;
  w.show();
  return QApplication::exec();
}
