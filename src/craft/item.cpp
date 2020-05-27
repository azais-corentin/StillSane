#include "item.hh"

#include <spdlog/spdlog.h>
#include <QRegularExpression>
#include <magic_enum.hpp>
#include <utility>

namespace StillSane::Craft {

Item::Item(QString fullText) : mFullText(std::move(fullText)) {
  // Split by sections and lines
  for (const auto& section : mFullText.splitRef("--------")) {
    const auto& lines = section.split("\r\n");
    mSectionsLines.emplace_back(lines.begin(), lines.end());
  }

  parse();
}

auto Item::unidentified() const -> bool {
  return mId == Identification::Unidentified;
}

auto Item::identified() const -> bool {
  return mId == Identification::Identified;
}

auto Item::rarity() const -> Item::Rarity {
  return mRarity;
}

auto Item::normal() const -> bool {
  return mRarity == Rarity::Normal;
}

auto Item::magic() const -> bool {
  return mRarity == Rarity::Magic;
}

auto Item::rare() const -> bool {
  return mRarity == Rarity::Rare;
}

auto Item::find(const std::string& text) -> bool {
  spdlog::debug("Finding: ", text, " = ",
                mFullText.contains(QString::fromStdString(text)));
  return mFullText.contains(QString::fromStdString(text));
}

auto Item::levelRequirement() const -> int {
  return mReqLevel;
}

auto Item::strRequirement() const -> int {
  return mReqStr;
}

auto Item::dexRequirement() const -> int {
  return mReqDex;
}

auto Item::intRequirement() const -> int {
  return mReqInt;
}

auto Item::sockets() const -> const std::vector<char>& {
  return mSockets;
}

auto Item::ilvl() const -> int {
  return mILvl;
}

auto Item::id() const -> Item::Identification {
  return mId;
}

void Item::operator=(const Item::Identification& id) {
  mId = id;
}

void Item::operator=(const Item::Rarity& rarity) {
  mRarity = rarity;
}

auto Item::operator==(const Item::Identification& id) const -> bool {
  return mId == id;
}

auto Item::operator==(const Item::Rarity& rarity) const -> bool {
  return mRarity == rarity;
}

template <typename T>
constexpr auto enumValue(const QString& name) -> T {
  const auto conversion = magic_enum::enum_cast<T>(name.toStdString());
  if (conversion.has_value()) {
    return conversion.value();
  }
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
  QRegularExpression patternProperty{
      R"(^(?'name'.+): (?'value'.+?)(?'effect' \([[:lower:]]+\))?$)", multiline};
  // auto it = patternProperty.globalMatch(mFullText);

  // Identification
  auto id = patternIdentification.match(mFullText);
  if (id.hasMatch()) {
    mId = Identified;
  } else {
    mId = Unidentified;
  }

  // Rarity
  auto rarity = patternRarity.match(mFullText);
  if (rarity.hasMatch()) {
    mRarity = enumValue<Rarity>(rarity.captured("rarity"));
  }

  // Requirements
  auto requirements = patterRequirements.match(mFullText);
  if (requirements.hasMatch()) {
    mReqLevel = requirements.capturedRef("level").toInt();
    mReqStr   = requirements.capturedRef("str").toInt();
    mReqDex   = requirements.capturedRef("dex").toInt();
    mReqInt   = requirements.capturedRef("int").toInt();
  }

  // Sockets
  // \todo get sockets

  // Item level
  auto ilvl = patternILvl.match(mFullText);
  if (ilvl.hasMatch()) {
    mILvl = ilvl.capturedRef("ilvl").toInt();
  }
}

}  // namespace StillSane::Craft
