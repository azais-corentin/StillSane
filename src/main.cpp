#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char* argv[]) {
  QCoreApplication::setOrganizationName("StillSane");
  QCoreApplication::setApplicationName("StillSane");

  QApplication a(argc, argv);

  StillSane::MainWindow w;
  w.show();
  return a.exec();
}
