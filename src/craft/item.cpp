#include "item.hh"

#include <QRegularExpression>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace StillSane::Craft {

Item::Item(const QString &fullText) : mFullText(fullText) {
    // Split by sections and lines
    for (const auto &section : mFullText.split("--------")) {
        const auto &lines = section.split("\r\n");
        mSectionsLines.emplace_back(lines.begin(), lines.end());
    }

    parse();
}

bool Item::unidentified() const { return mId == Identification::Unidentified; }

bool Item::identified() const { return mId == Identification::Identified; }

Item::Rarity Item::rarity() const { return mRarity; }

bool Item::normal() const { return mRarity == Rarity::Normal; }

bool Item::magic() const { return mRarity == Rarity::Magic; }

bool Item::rare() const { return mRarity == Rarity::Rare; }

bool Item::find(const std::string &text) {
    spdlog::debug("Finding: ", text, " = ", mFullText.contains(QString::fromStdString(text)));
    return mFullText.contains(QString::fromStdString(text));
}

int Item::levelRequirement() const { return mReqLevel; }

int Item::strRequirement() const { return mReqStr; }

int Item::dexRequirement() const { return mReqDex; }

int Item::intRequirement() const { return mReqInt; }

const std::vector<char> &Item::sockets() const { return mSockets; }

int Item::ilvl() const { return mILvl; }

Item::Identification Item::id() const { return mId; }

void Item::operator=(const Item::Identification &id) { mId = id; }

void Item::operator=(const Item::Rarity &rarity) { mRarity = rarity; }

bool Item::operator==(const Item::Identification &id) const { return mId == id; }

bool Item::operator==(const Item::Rarity &rarity) const { return mRarity == rarity; }

template <typename T> constexpr T enumValue(const QString &name) {
    const auto conversion = magic_enum::enum_cast<T>(name.toStdString());
    if (conversion.has_value()) return conversion.value();
    return static_cast<T>(Item::Unknown);
}

void Item::parse() {
    constexpr auto multiline = QRegularExpression::MultilineOption;

    QRegularExpression patternIdentification{R"(^Unidentified$)", multiline};
    QRegularExpression patternRarity{R"(^Rarity: (?'rarity'.+)$)", multiline};
    QRegularExpression patternILvl{R"(^Item Level: (?'ilvl'\d+)$)", multiline};
    QRegularExpression patterRequirements{
        R"(^Requirements:\n(?>^Level: (?'level'\d+)$\n)?(?>^Str: (?'str'\d+)$\n)?(?>^Dex: (?'dex'\d+)$\n)?(?>^Int: (?'int'\d+)$\n)?)",
        multiline};
    // Matches format: "words: value (augmented or enchant or implicit)?"
    QRegularExpression patternProperty{R"(^(?'name'.+): (?'value'.+?)(?'effect' \([[:lower:]]+\))?$)", multiline};
    // auto it = patternProperty.globalMatch(mFullText);

    // Identification
    auto id = patternIdentification.match(mFullText);
    if (id.hasMatch())
        mId = Identified;
    else
        mId = Unidentified;

    // Rarity
    auto rarity = patternRarity.match(mFullText);
    if (rarity.hasMatch()) { mRarity = enumValue<Rarity>(rarity.captured("rarity")); }

    // Requirements
    auto requirements = patterRequirements.match(mFullText);
    if (requirements.hasMatch()) {
        mReqLevel = requirements.captured("level").toInt();
        mReqStr   = requirements.captured("str").toInt();
        mReqDex   = requirements.captured("dex").toInt();
        mReqInt   = requirements.captured("int").toInt();
    }

    // Sockets
    // \todo get sockets

    // Item level
    auto ilvl = patternILvl.match(mFullText);
    if (ilvl.hasMatch()) { mILvl = ilvl.captured("ilvl").toInt(); }
}

} // namespace StillSane::Craft
