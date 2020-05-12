#ifndef AUTOTRADE_CRAFT_CRAFTER_HH
#define AUTOTRADE_CRAFT_CRAFTER_HH

#include <any>
#include <memory>

#include <QObject>
#include <QPoint>
#include <QTimer>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace AutoTrade::Craft {

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

enum class ItemState { Unknown, Unidentified, Identified };
enum class ItemRarity { Unknown, Normal, Magic, Rare };

class Crafter : public QObject {
  Q_OBJECT

 public:
  explicit Crafter(QObject* parent = nullptr);
  ~Crafter();

  void start(const QString& script);
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

  bool is_unidentified();
  bool is_normal();
  bool is_magic();
  bool is_rare();
  bool is_matching();

  bool matches(std::string text);

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

 private:
  sol::state              mLua;
  sol::protected_function mIsMatching;

  QString mMatchScript;
  QTimer  mTimer;
  bool    mRunning = false;

  ItemState   mState  = ItemState::Unknown;
  ItemRarity  mRarity = ItemRarity::Unknown;
  QStringList mLines;
  // QStringList mExplicits;

  // std::any mMachine;
  // std::unique_ptr<boost::sml::sm<StateMachines::AlterationOnly>> mMachine;
};

}  // namespace AutoTrade::Craft

#endif  // AUTOTRADE_CRAFT_CRAFTER_HH
