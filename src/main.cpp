#include "mainwindow.hh"

#include <QApplication>
#include <QStyleFactory>

auto createDarkPalette() -> QPalette {
  const auto shadowBackground        = QColor{"#ff404142"};
  const auto text                    = QColor{"#ffd0d0d0"};
  const auto textDisabled            = QColor{"#60a4a6a8"};
  const auto textHighlighted         = QColor{"#fff0f0f0"};
  const auto selectedBackgroundText  = QColor{"#ff1d545c"};
  const auto normalBackground        = QColor{"#ff2E2F30"};
  const auto alternateBackground     = QColor{"#ff353637"};
  const auto textColorLink           = QColor{"#ff007af4"};
  const auto textColorLinkVisited    = QColor{"#ffa57aff"};
  const auto backgroundColorDisabled = QColor{"#ff444444"};

  QPalette palette;
  palette.setColor(QPalette::Window, normalBackground);
  palette.setColor(QPalette::WindowText, text);
  palette.setColor(QPalette::Base, normalBackground);
  palette.setColor(QPalette::AlternateBase, alternateBackground);
  palette.setColor(QPalette::Button, shadowBackground);
  palette.setColor(QPalette::BrightText, QColor{"#ffff3333"});
  palette.setColor(QPalette::Text, text);
  palette.setColor(QPalette::ButtonText, text);
  palette.setColor(QPalette::ToolTipBase, shadowBackground);
  palette.setColor(QPalette::Highlight, selectedBackgroundText);
  palette.setColor(QPalette::Dark, shadowBackground);
  palette.setColor(QPalette::HighlightedText, textHighlighted);
  palette.setColor(QPalette::ToolTipText, text);
  palette.setColor(QPalette::Link, textColorLink);
  palette.setColor(QPalette::LinkVisited, textColorLinkVisited);

  palette.setColor(QPalette::Disabled, QPalette::ButtonText, textDisabled);
  palette.setColor(QPalette::Disabled, QPalette::Window, backgroundColorDisabled);
  palette.setColor(QPalette::Disabled, QPalette::WindowText, textDisabled);
  palette.setColor(QPalette::Disabled, QPalette::Base, backgroundColorDisabled);
  palette.setColor(QPalette::Disabled, QPalette::Text, textDisabled);

  return palette;
}

/*auto createDarkPalette() -> QPalette {
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
  palette.setColor(QPalette::Dark, darkGray);  // or darkGrey
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
}*/

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
