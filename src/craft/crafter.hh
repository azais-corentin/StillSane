#pragma once

#include <any>
#include <memory>

#include <QObject>
#include <QPoint>
#include <QTimer>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/forward.hpp>

#include "fsm.hh"
#include "item.hh"

namespace StillSane::Craft {

enum class MouseRegion {
  Item,
  Transmutation,
  Alteration,
  Annulment,
  Chance,
  Exalted,
  Regal,
  Alchemy,
  Chaos,
  Blessed,
  Scouring,
  Augmentation
};
enum class MouseButton { Left, Right };

class Crafter : public QObject {
  Q_OBJECT

 public:
  explicit Crafter(QObject* parent = nullptr);
  ~Crafter();

  void start(const std::string& transitionTable, const std::string& functions);
  void stop();

  //// Actions
  void copy();
  void parse();
  // Orbs
  void transmutation();
  void alteration();
  void annulment();
  void chance();
  void exalted();
  void regal();
  void alchemy();
  void chaos();
  void blessed();
  void scouring();
  void augmentation();

 signals:
  void error(const QString& error);
  void info(const QString& message);

  void foundExplicit(const QString& explicitModifier);

 private:
  void applyOrb(MouseRegion orb);

  void mouseMove(const MouseRegion& region);
  void mousePress(const MouseButton& button);
  void keyPress(const unsigned short& key);
  void keyRelease(const unsigned short& key);

  QPoint getPosition(const MouseRegion& region);
  void   standardDelay();

  void resetLuaState();

 private:
  std::shared_ptr<sol::state> mLuaState;
  std::unique_ptr<FSM>        mFSM;

  std::string mCodeFunctions, mCodeTransitionTable;
  QTimer      mTimer;
  bool        mRunning = false;

  Item mItem;
  // QStringList mExplicits;
};

}  // namespace StillSane::Craft
