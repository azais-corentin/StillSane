#pragma once

namespace StillSane::Modules::Write::Mouse {

enum Button { Left, Right };

enum Region {
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

void move(const Region& region);
void move(const Region& region);

}  // namespace StillSane::Modules::Write::Mouse
