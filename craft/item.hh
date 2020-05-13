#pragma once

#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <QString>

namespace AutoTrade::Craft {

class Item {
 public:
  static constexpr uint8_t Unknown = std::numeric_limits<uint8_t>::max();
  enum Identification : uint8_t { Unidentified, Identified };
  enum Rarity : uint8_t { Normal, Magic, Rare, Unique };

 public:
  Item() = default;
  Item(const QString& fullText);
  // Disable copy
  Item(const Item&) = delete;
  Item& operator=(const Item&) = delete;
  // Enable move construction & assignment
  Item(Item&&)  = default;
  Item& operator=(Item&&) = default;

  bool find(const std::string& text);

  bool unidentified();
  bool identified();

  // Rarity
  Rarity rarity();
  bool   normal();
  bool   magic();
  bool   rare();

  // Requirements
  int levelRequirement() { return mReqLevel; }
  int strRequirement() { return mReqStr; }
  int dexRequirement() { return mReqDex; }
  int intRequirement() { return mReqInt; }

  // Sockets
  const std::vector<char>& sockets() { return mSockets; }

  // Item level
  int ilvl() { return mILvl; }

  // Assignments
  void operator=(const Identification& id);
  void operator=(const Rarity& rarity);

  // Comparisons
  bool operator==(const Rarity& rarity);
  bool operator==(const Identification& id);

 private:
  void parse();

 private:
  QString                              mFullText;
  std::vector<std::vector<QStringRef>> mSectionsLines;

  // Default values
  Identification    mId{Unknown};
  Rarity            mRarity{Unknown};
  int               mReqLevel = 0, mReqStr = 0, mReqDex = 0, mReqInt = 0;
  std::vector<char> mSockets;
  int               mILvl;
};

}  // namespace AutoTrade::Craft
