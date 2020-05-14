#include "crafter.hh"

#include <filesystem>
#include <thread>
#include "windows.h"

#include <QApplication>
#include <QClipboard>
#include <QRegularExpression>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <magic_enum.hpp>
#include <range/v3/view/filter.hpp>
#include <sol/sol.hpp>

namespace AutoTrade::Craft {

namespace fs = std::filesystem;

Crafter::Crafter(QObject* parent) : QObject(parent) {
  auto tables_path = fs::current_path() / "resources" / "transition_tables";

  auto entries_it = fs::directory_iterator(tables_path);

  std::vector<fs::directory_entry> entries{fs::begin(entries_it), fs::end(entries_it)};

  auto entries_lua = entries | ranges::views::filter([](fs::directory_entry& entry) {
                       return entry.path().extension() == ".lua";
                     });

  connect(&mTimer, &QTimer::timeout, [&]() {
    if (mRunning) {
      mFSM->process_event("next");
      if (mFSM->finished()) {
        stop();
      }
    }
  });
}

Crafter::~Crafter() {
  // The destructor needs to be in crafter.cpp for the unique_ptr to work.
}

void Crafter::start(const std::string& transitionTable, const std::string& functions) {
  mCodeTransitionTable = transitionTable;
  mCodeFunctions       = functions;

  resetLuaState();
  mFSM = std::make_unique<FSM>(mLuaState);

  // mMachine = std::make_unique<sml::sm<StateMachines::AlterationOnly>>(*this);

  // Execute functions
  {
    auto result = mLuaState->safe_script(mCodeFunctions, sol::script_pass_on_error);
    if (!result.valid()) {
      sol::error err = result;
      emit       error(tr("Functions script error") + ": " + err.what());
      stop();
      return;
    }
  }

  // Execute transition table
  {
    auto result = mLuaState->safe_script(mCodeTransitionTable, sol::script_pass_on_error);
    if (!result.valid()) {
      sol::error err = result;
      emit       error(tr("Transition table script error") + ": " + err.what());
      stop();
      return;
    } else {
      sol::optional<std::vector<std::string>> value = result;
      mFSM->parse(*value);
    }
  }

  emit info(tr("Valid script, starting craft"));

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

  try {
    mItem = Item{text};
  } catch (const std::runtime_error& e) {
    error(QStringLiteral("Error parsing item text: ") + e.what());
    return;
  }

  // Extract rarity
  auto rarity = QRegularExpression("^Rarity: (?'rarity'[a-zA-Z]+)$",
                                   QRegularExpression::MultilineOption)
                    .match(text);
  if (rarity.hasMatch()) {
    auto rarityText = rarity.captured("rarity");
    if (rarityText == "Normal")
      mItem = Item::Normal;
    else if (rarityText == "Magic")
      mItem = Item::Magic;
    else if (rarityText == "Rare")
      mItem = Item::Rare;
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
  std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void Crafter::resetLuaState() {
  // Reset lua state
  mLuaState = std::make_shared<sol::state>();
  mLuaState->open_libraries(sol::lib::base);

  //  mLuaState->set_function(
  //      "debug", sol::overload([](const std::string& msg) { spdlog::debug(msg); },
  //                             [](const double& number) { spdlog::debug(number); }));
  mLuaState->set_function("debug", [](sol::variadic_args args) {
    std::vector<std::string> args_string;
    for (const auto& arg : args)
      args_string.emplace_back(arg.get<std::string>());

    spdlog::debug("{}", fmt::join(args_string.begin(), args_string.end(), " "));
  });

  // Set item
  mLuaState->set("item", &mItem);

  // Set crafter context
  mLuaState->set_function("copy", &Crafter::copy, this);
  mLuaState->set_function("parse", &Crafter::parse, this);
  mLuaState->set_function("transmutation", &Crafter::transmutation, this);
  mLuaState->set_function("alteration", &Crafter::alteration, this);
  mLuaState->set_function("annulment", &Crafter::annulment, this);
  mLuaState->set_function("chance", &Crafter::chance, this);
  mLuaState->set_function("exalted", &Crafter::exalted, this);
  mLuaState->set_function("regal", &Crafter::regal, this);
  mLuaState->set_function("alchemy", &Crafter::alchemy, this);
  mLuaState->set_function("chaos", &Crafter::chaos, this);
  mLuaState->set_function("blessed", &Crafter::blessed, this);
  mLuaState->set_function("scouring", &Crafter::scouring, this);
  mLuaState->set_function("augmentation", &Crafter::augmentation, this);

  // Set item context
  mLuaState->set("Unknown", Item::Unknown);
  auto item_type = mLuaState->new_usertype<Item>(
      "Item",
      // Find
      "find", &Item::find,  //
      // Identification
      "id", sol::property(&Item::id),                      //
      "unidentified", sol::property(&Item::unidentified),  //
      "identified", sol::property(&Item::identified),      //
      // Rarity
      "rarity", sol::property(&Item::rarity),  //
      "normal", sol::property(&Item::normal),  //
      "magic", sol::property(&Item::magic),    //
      "rare", sol::property(&Item::rare)       //
  );

  mLuaState->set("Unidentified", Item::Unidentified);
  mLuaState->set("Identified", Item::Identified);
  mLuaState->new_enum("Identification",                    //
                      "Unidentified", Item::Unidentified,  //
                      "Identified", Item::Identified       //
  );
  mLuaState->set("Normal", Item::Normal);
  mLuaState->set("Magic", Item::Magic);
  mLuaState->set("Rare", Item::Rare);
  mLuaState->set("Unique", Item::Unique);
  mLuaState->new_enum("Rarity",                //
                      "Normal", Item::Normal,  //
                      "Magic", Item::Magic,    //
                      "Rare", Item::Rare,      //
                      "Unique", Item::Unique   //
  );
}

}  // namespace AutoTrade::Craft
