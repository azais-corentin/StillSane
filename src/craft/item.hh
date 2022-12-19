#pragma once

#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <vector>

#include <QString>

namespace StillSane::Craft {

class Item {
  public:
    static constexpr uint8_t Unknown = std::numeric_limits<uint8_t>::max();
    enum Identification : uint8_t
    {
        Unidentified,
        Identified
    };
    enum Rarity : uint8_t
    {
        Normal,
        Magic,
        Rare,
        Unique
    };

  public:
    Item() = default;
    Item(const QString &fullText);
    // Disable copy
    Item(const Item &) = delete;
    Item &operator=(const Item &) = delete;
    // Enable move construction & assignment
    Item(Item &&) = default;
    Item &operator=(Item &&) = default;

    bool find(const std::string &text);

    // Identification
    Identification id() const;
    bool unidentified() const;
    bool identified() const;

    // Rarity
    Rarity rarity() const;
    bool normal() const;
    bool magic() const;
    bool rare() const;

    // Requirements
    int levelRequirement() const;
    int strRequirement() const;
    int dexRequirement() const;
    int intRequirement() const;

    // Sockets
    const std::vector<char> &sockets() const;

    // Item level
    int ilvl() const;

    // Assignments
    void operator=(const Identification &id);
    void operator=(const Rarity &rarity);

    // Comparisons
    bool operator==(const Rarity &rarity) const;
    bool operator==(const Identification &id) const;

  private:
    void parse();

  private:
    QString mFullText;
    std::vector<std::vector<QString>> mSectionsLines;

    // Default values
    Identification mId{static_cast<Identification>(Unknown)};
    Rarity mRarity{static_cast<Rarity>(Unknown)};
    int mReqLevel = 0, mReqStr = 0, mReqDex = 0, mReqInt = 0;
    std::vector<char> mSockets;
    int mILvl;
};

} // namespace StillSane::Craft
