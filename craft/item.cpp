#include "item.hh"

#include <QRegularExpression>

#include <magic_enum.hpp>

namespace AutoTrade::Craft {

Item::Item(const QString& fullText) : mFullText(fullText) {
  // Fill sections and lines vectors
  for (const auto& section : mFullText.splitRef("--------")) {
    const auto& lines = section.split("\r\n");
    mSectionsLines.emplace_back(lines.begin(), lines.end());
  }

  parse();
}

bool Item::unidentified() {
  return mId == Identification::Unidentified;
}

bool Item::identified() {
  return mId == Identification::Identified;
}

Item::Rarity Item::rarity() {
  return mRarity;
}

bool Item::normal() {
  return mRarity == Rarity::Normal;
}

bool Item::magic() {
  return mRarity == Rarity::Magic;
}

bool Item::rare() {
  return mRarity == Rarity::Rare;
}

void Item::operator=(const Item::Identification& id) {
  mId = id;
}

bool Item::find(const std::string& text) {
  return mFullText.contains(QString::fromStdString(text));
}

void Item::operator=(const Item::Rarity& rarity) {
  mRarity = rarity;
}

bool Item::operator==(const Item::Rarity& rarity) {
  return mRarity = rarity;
}

bool Item::operator==(const Item::Identification& id) {
  return mId = id;
}

template <typename T>
constexpr T enumValue(const QString& name) {
  const auto conversion = magic_enum::enum_cast<T>(name.toStdString());
  if (conversion.has_value())
    return conversion.value();
  return static_cast<T>(Item::Unknown);
}

void Item::parse() {
  constexpr auto multiline = QRegularExpression::MultilineOption;

  QRegularExpression patternRarity{R"(^Rarity: (.+)$)", multiline};
  QRegularExpression patternILvl{R"(^Item Level: (\d+/$)", multiline};
  QRegularExpression patterRequirements{
      R"(^Requirements:\n(?>^Level: (?'level'\d+)$\n)?(?>^Str: (?'str'\d+)$\n)?(?>^Dex: (?'dex'\d+)$\n)?(?>^Int: (?'int'\d+)$\n)?)",
      multiline};
  // Matches format: "words: value (augmented or enchant or implicit)?"
  QRegularExpression patternProperty{
      R"(^(?'name'.+): (?'value'.+?)(?'effect' \([[:lower:]]+\))?$))", multiline};
  // auto it = patternProperty.globalMatch(mFullText);

  // Rarity
  auto rarity = patternRarity.match(mFullText);
  if (rarity.hasMatch()) {
    mRarity = enumValue<Rarity>(rarity.captured());
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
    mILvl = ilvl.capturedRef().toInt();
  }
}

}  // namespace AutoTrade::Craft
