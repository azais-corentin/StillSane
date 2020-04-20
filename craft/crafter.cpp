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
      mMachine->process_event("next"_e());
    }
  });
}

Crafter::~Crafter() {
  // The destructor needs to be in crafter.cpp for the unique_ptr to work.
}

void Crafter::start(const QString& script) {
  mMatchScript = script;

  parse();
  mMachine = std::make_unique<sml::sm<StateMachines::AlterationOnly>>(*this);

  auto result = mLua.safe_script(mMatchScript.toStdString(), sol::script_pass_on_error);
  if (!result.valid()) {
    emit error(tr("Invalid script: ") + sol::error(result).what());
    stop();
    return;
  } else {
    emit info(tr("Valid script, starting craft"));
  }

  mIsMatching = mLua["is_matching"];
  if (!mIsMatching.valid()) {
    qDebug() << "Couldn't find function!";
    emit error(tr("Couldn't find function is_matching() in lua script"));
    stop();
    return;
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
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // Copy item data
  keyPress(VK_CONTROL);
  keyPress('C');
  standardDelay();
  keyRelease('C');
  keyRelease(VK_CONTROL);
  standardDelay();
}

void Crafter::parse() {
  bool           needStop = false;
  const QString& text     = QApplication::clipboard()->text();

  // Save all lines
  mLines.clear();
  mLines.append(text.split('\n'));

  // Extract rarity
  mRarity          = ItemRarity::Unknown;
  auto rarityMatch = QRegularExpression("^Rarity: (?'rarity'[a-zA-Z]+)$",
                                        QRegularExpression::MultilineOption)
                         .match(text);
  if (rarityMatch.hasMatch()) {
    auto rarityText = rarityMatch.captured("rarity");
    if (rarityText == "Normal")
      mRarity = ItemRarity::Normal;
    else if (rarityText == "Magic")
      mRarity = ItemRarity::Magic;
    else if (rarityText == "Rare")
      mRarity = ItemRarity::Rare;
    else {
      emit error("Invalid rarity:" + rarityText);
      needStop = true;
    }
  } else {
    emit error("Could not find rarity in text:\n" + text);
    needStop = true;
  }

  if (needStop)
    stop();
}

void Crafter::transmutation() {
  applyOrb(MouseRegion::Transmutation);
}

void Crafter::alteration() {
  applyOrb(MouseRegion::Alteration);
}

void Crafter::annulment() {
  applyOrb(MouseRegion::Annulment);
}

void Crafter::chance() {
  applyOrb(MouseRegion::Chance);
}

void Crafter::exalted() {
  applyOrb(MouseRegion::Exalted);
}

void Crafter::regal() {
  applyOrb(MouseRegion::Regal);
}

void Crafter::alchemy() {
  applyOrb(MouseRegion::Alchemy);
}

void Crafter::chaos() {
  applyOrb(MouseRegion::Chaos);
}

void Crafter::blessed() {
  applyOrb(MouseRegion::Blessed);
}

void Crafter::scouring() {
  applyOrb(MouseRegion::Scouring);
}

void Crafter::augmentation() {
  applyOrb(MouseRegion::Augmentation);
}

bool Crafter::is_unidentified() {
  return mState == ItemState::Unidentified;
}

bool Crafter::is_normal() {
  return mRarity == ItemRarity::Normal;
}

bool Crafter::is_magic() {
  return mRarity == ItemRarity::Magic;
}

bool Crafter::is_rare() {
  return mRarity == ItemRarity::Rare;
}

bool Crafter::is_matching() {
  qDebug() << "Crafter::is_matching";
  sol::protected_function_result itemMatched = mIsMatching();

  if (itemMatched.valid()) {
    qDebug() << "matching:" << itemMatched.get<bool>();
    return itemMatched;
  } else {
    error("Crafter::is_matching error: invalid result from function is_matching()");
  }

  return false;
}

bool Crafter::matches(std::string text) {
  return mLines.filter(QString::fromStdString(text), Qt::CaseInsensitive).size() != 0;
}

void Crafter::applyOrb(MouseRegion orb) {
  mouseMove(orb);
  standardDelay();
  mousePress(MouseButton::Right);
  standardDelay();
  mouseMove(MouseRegion::Item);
  standardDelay();
  mousePress(MouseButton::Left);
  standardDelay();
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
      return {59, 291};
    case MouseRegion::Alteration:
      return {117, 291};
    case MouseRegion::Annulment:
      return {175, 291};
    case MouseRegion::Chance:
      return {233, 291};
    case MouseRegion::Exalted:
      return {303, 291};
    case MouseRegion::Regal:
      return {431, 291};
    case MouseRegion::Alchemy:
      return {489, 291};
    case MouseRegion::Chaos:
      return {547, 291};
    case MouseRegion::Blessed:
      return {606, 291};
    case MouseRegion::Scouring:
      return {176, 475};
    case MouseRegion::Augmentation:
      return {233, 348};
  }

  return {};
}

void Crafter::standardDelay() {
  std::this_thread::sleep_for(std::chrono::milliseconds(12));
}

}  // namespace AutoTrade::Craft
