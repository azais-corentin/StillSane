return {
  "*parse + next                  / (copy, parse)                 = parsed",
  "parsed + next [ is_normal ]                                    = normal",
  "parsed + next [ is_magic ]                                     = magic",
  "parsed + next [ is_rare ]                                      = rare",

  "normal + next                  / (transmutation, augmentation) = parse",
  "magic  + next [ !is_matching ] / (alteration, augmentation)    = parse",
  "magic  + next [ is_matching ]  / regal                         = X",
  "rare   + next [ !is_matching ] / scouring                      = parse",
  "rare   + next [ is_matching ]                                  = X"
}