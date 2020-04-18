#include "crafter.hh"

#include <thread>
#include "windows.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QRegularExpression>

#include <craft/statemachines.hh>

namespace AutoTrade::Craft {

namespace sml = boost::sml;

Crafter::Crafter(QObject* parent) : QObject(parent) {
  mLua.open_libraries(sol::lib::base);
  mLua.set_function("matches", &Crafter::matches, this);

  connect(&mTimer, &QTimer::timeout, [&]() {
    using namespace sml;

    if (mRunning) {
      std::any_cast<sm<StateMachines::AlterationOnly>>(mMachine).process_event(
          "next"_e());
    }
  });
}

void Crafter::start(const QString& script) {
  mMatchScript = script;

  parse();
  mMachine = std::make_any<sml::sm<StateMachines::AlterationOnly>>(*this);
  // std::any_cast<sml::sm<StateMachines::AlterationOnly>>(mMachine);

  auto result = mLua.safe_script(mMatchScript.toStdString(), sol::script_pass_on_error);
  if (!result.valid()) {
    emit scriptError(tr("Invalid script: ") + sol::error(result).what());
  } else {
    emit scriptMessage(tr("Valid script"));
  }

  mRunning = true;
  mTimer.start(0);
}

void Crafter::stop() {
  mRunning = false;
  mTimer.stop();
}

void Crafter::copy() {
  // Select item
  mouseMove(MouseRegion::Item);
  standardDelay();
  // Copy item data
  keyPress(VK_CONTROL);
  keyPress('C');
  standardDelay();
  keyRelease('C');
  keyRelease(VK_CONTROL);
  standardDelay();
}

void Crafter::parse() {
  const QString& text = QApplication::clipboard()->text();

  const auto& categories = text.splitRef("--------\n");
  if (categories.size() < 4) {
    qDebug() << "not enough categories:" << text;
    return;
  }

  // Save all lines
  mLines.clear();
  for (const auto& category : categories) {
    mLines.append(category.toString().split('\n'));
  }

  const auto& rarityCategory = categories.at(0);
  const auto& rarityTokens   = rarityCategory.split('\n');
  const auto& rarity         = rarityTokens.at(0);

  if (!rarity.startsWith("Rarity: ")) {
    qDebug() << "invalid rarity:" << rarity;
    return;
  }

  if (rarity.endsWith("Normal"))
    mState = ItemState::Normal;
  else if (rarity.endsWith("Magic"))
    mState = ItemState::Magic;
  else if (rarity.endsWith("Rare"))
    mState = ItemState::Rare;
  else {
    mState = ItemState::Unidentified;
    qDebug() << "invalid rarity:" << rarity;
    return;
  }

  if (mState >= ItemState::Magic) {
    int count = categories.size();

    const auto& explicitsCategory = categories.at(count - 2).toString();
    mExplicits                    = explicitsCategory.split('\n');

    for (auto explicitMod : mExplicits) {
      emit foundExplicit(explicitMod);
    }

    qDebug() << "found explicits:" << mExplicits;
  }
}

void Crafter::transmutation() {
  mouseMove(MouseRegion::Transmutation);
  standardDelay();
  mousePress(MouseButton::Right);
  standardDelay();
  mouseMove(MouseRegion::Item);
  standardDelay();
  mousePress(MouseButton::Left);
  standardDelay();
}

void Crafter::alteration() {
  mouseMove(MouseRegion::Alteration);
  standardDelay();
  mousePress(MouseButton::Right);
  standardDelay();
  mouseMove(MouseRegion::Item);
  standardDelay();
  mousePress(MouseButton::Left);
  standardDelay();
}

void Crafter::regal() {
  mouseMove(MouseRegion::Regal);
  standardDelay();
  mousePress(MouseButton::Right);
  standardDelay();
  mouseMove(MouseRegion::Item);
  standardDelay();
  mousePress(MouseButton::Left);
  standardDelay();
}

void Crafter::scouring() {
  mouseMove(MouseRegion::Scouring);
  standardDelay();
  mousePress(MouseButton::Right);
  standardDelay();
  mouseMove(MouseRegion::Item);
  standardDelay();
  mousePress(MouseButton::Left);
  standardDelay();
}

bool Crafter::is_unidentified() {
  return mState == ItemState::Unidentified;
}

bool Crafter::is_normal() {
  return mState == ItemState::Normal;
}

bool Crafter::is_magic() {
  return mState == ItemState::Magic;
}

bool Crafter::is_rare() {
  return mState == ItemState::Rare;
}

bool Crafter::is_matching() {
  QStringList matchingLists = mExplicits.filter("Fettle");
  bool        matching      = matchingLists.size() != 0;

  if (matching) {
    qDebug() << "Matching explicit modifier(s):" << matchingLists;
  }
  return matching;
}

bool Crafter::matches(std::string text) {
  return mLines.filter(QString::fromStdString(text)).size() != 0;
}

void Crafter::mouseMove(const MouseRegion& region) {
  INPUT ip;

  QPoint pos = getPosition(region);

  ip.type         = INPUT_MOUSE;
  ip.mi.mouseData = 0;
  ip.mi.time      = 0;
  ip.mi.dx        = pos.x() * (65536 / GetSystemMetrics(SM_CXSCREEN));
  ip.mi.dy        = pos.y() * (65536 / GetSystemMetrics(SM_CYSCREEN));
  ip.mi.dwFlags   = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

  SendInput(1, &ip, sizeof(ip));
}

void Crafter::mousePress(const MouseButton& button) {
  INPUT ip;

  ip.type           = INPUT_MOUSE;
  ip.mi.mouseData   = 0;
  ip.mi.time        = 0;
  ip.mi.dx          = 0;
  ip.mi.dy          = 0;
  ip.mi.dwExtraInfo = 0;
  if (button == MouseButton::Left)
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
  else if (button == MouseButton::Right)
    ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;

  SendInput(1, &ip, sizeof(ip));
}

void Crafter::keyPress(const unsigned short& key) {
  INPUT ip;
  ip.type           = INPUT_KEYBOARD;
  ip.ki.wVk         = key;
  ip.ki.wScan       = 0;
  ip.ki.time        = 0;
  ip.ki.dwExtraInfo = 0;
  ip.ki.dwFlags     = 0;
  SendInput(1, &ip, sizeof(ip));
}

void Crafter::keyRelease(const unsigned short& key) {
  INPUT ip;
  ip.type           = INPUT_KEYBOARD;
  ip.ki.wVk         = key;
  ip.ki.wScan       = 0;
  ip.ki.time        = 0;
  ip.ki.dwExtraInfo = 0;
  ip.ki.dwFlags     = KEYEVENTF_KEYUP;
  SendInput(1, &ip, sizeof(ip));
}

QPoint Crafter::getPosition(const MouseRegion& region) {
  switch (region) {
    case MouseRegion::Item:
      return {333, 480};
    case MouseRegion::Transmutation:
      return {59, 292};
    case MouseRegion::Alteration:
      return {117, 292};
    case MouseRegion::Regal:
      return {431, 290};
    case MouseRegion::Scouring:
      return {176, 475};
  }

  return {};
}

void Crafter::standardDelay() {
  std::this_thread::sleep_for(std::chrono::milliseconds(40));
}

}  // namespace AutoTrade::Craft
