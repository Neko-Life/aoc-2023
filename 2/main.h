#ifndef MAIN_H
#define MAIN_H

#include <vector>

struct game_set_t {
  int r;
  int g;
  int b;
};

struct set_item_t {
  int amount;
  // r g b
  char type;
};

struct game_t {
  int id;
  std::vector<game_set_t> sets;
};

#endif // MAIN_H
