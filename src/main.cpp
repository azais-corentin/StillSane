#include "mainwindow.hh"

#include <QApplication>
#include <QStyleFactory>

auto createDarkPalette() -> QPalette {
  constexpr QColor gray{127, 127, 127};
  constexpr QColor darkGray{53, 53, 53};
  constexpr QColor black{25, 25, 25};
  constexpr QColor blue{42, 130, 218};

  QPalette palette;
  palette.setColor(QPalette::Window, darkGray);
  palette.setColor(QPalette::WindowText, Qt::white);
  palette.setColor(QPalette::Base, black);
  palette.setColor(QPalette::AlternateBase, darkGray);
  palette.setColor(QPalette::ToolTipBase, Qt::white);
  palette.setColor(QPalette::ToolTipText, Qt::white);
  palette.setColor(QPalette::Text, Qt::white);
  palette.setColor(QPalette::Dark, black);  // or darkGrey
  palette.setColor(QPalette::Shadow, black);
  palette.setColor(QPalette::Button, darkGray);
  palette.setColor(QPalette::ButtonText, Qt::white);
  palette.setColor(QPalette::BrightText, Qt::red);
  palette.setColor(QPalette::Link, blue);
  palette.setColor(QPalette::Highlight, blue);
  palette.setColor(QPalette::HighlightedText, Qt::black);

  palette.setColor(QPalette::Active, QPalette::Button, gray.darker());
  palette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
  palette.setColor(QPalette::Disabled, QPalette::Text, gray);
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
  palette.setColor(QPalette::Disabled, QPalette::Light, gray);
  return palette;
}

auto main(int argc, char* argv[]) -> int {
  QCoreApplication::setOrganizationName("StillSane");
  QCoreApplication::setApplicationName("StillSane");

  QApplication a(argc, argv);

  a.setStyle(QStyleFactory::create("Fusion"));
  a.setPalette(createDarkPalette());
  a.setStyleSheet(
      "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

  StillSane::MainWindow w;
  w.show();
  return QApplication::exec();
}
