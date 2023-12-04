#include "main.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

constexpr char digits[] = "0987654321";
// 12 red cubes, 13 green cubes, and 14 blue
constexpr game_set_t bag = {12, 13, 14};

bool is_digit(char c) {
  for (char comp : digits) {
    if (comp == c)
      return true;
  }

  return false;
}

int get_game_id(const char *str) {
  constexpr size_t jump_idx = sizeof("Game ") - 1;

  char res[10];
  size_t res_siz = 0;
  for (size_t i = jump_idx; is_digit(str[i]); i++) {
    if (res_siz == 10)
      // input error! no way there's 10 digit there!
      return -1;

    res[i - jump_idx] = str[i];
    res_siz++;
  }

  res[res_siz] = '\0';

  return atoi(res);
}

void parse_set(const char *str, set_item_t *set) {
  if (!set)
    return;

  char amo[10];
  size_t amo_siz = 0;
  bool got_amount = false;

  char typ[10];
  size_t typ_siz = 0;

  for (size_t i = 0; str[i] && str[i] != ',' && str[i] != ';' && str[i] != '\n';
       i++) {
    char c = str[i];

    if (!got_amount) {
      if (amo_siz == 10)
        // 10 digit??
        return;

      if (!is_digit(c)) {
        got_amount = true;
        amo[amo_siz] = '\0';

#ifdef DEBUG
        std::cerr << "amo_siz(" << amo_siz << ")\n";
        std::cerr << "amo(" << amo << ")\n";
#endif

        continue;
      }

      amo[amo_siz++] = c;

      continue;
    }

    if (typ_siz == 10)
      return;

    typ[typ_siz++] = c;
  }

  typ[typ_siz] = '\0';

#ifdef DEBUG
  std::cerr << "typ_siz(" << typ_siz << ")\n";
  std::cerr << "typ(" << typ << ")\n";
#endif

  set->amount = atoi(amo);

  if (strcmp(typ, "red") == 0) {
    set->type = 'r';
  } else if (strcmp(typ, "green") == 0) {
    set->type = 'g';
  } else if (strcmp(typ, "blue") == 0) {
    set->type = 'b';
  }
}

int main() {
  FILE *input = fopen("input.txt", "r");
  if (!input) {
    perror("fopen");
    return 1;
  }

  fseek(input, 0, SEEK_END);
  long fsiz = ftell(input);
  fseek(input, 0, SEEK_SET);

  char *buf = (char *)malloc(sizeof(char) * (fsiz + 1));
  ssize_t readsiz = fread(buf, 1, fsiz, input);
  buf[readsiz] = '\0';
  fclose(input);

  std::string input_str(buf);
  free(buf);

  size_t input_len = input_str.length();

  std::vector<game_t> games;

  game_t game = {-1};
  game_set_t game_set = {0, 0, 0};

  bool lnstart = true;
  bool should_parse_set = false;
  for (size_t i = 0; i < input_len; i++) {
    char c = input_str.at(i);

    if (lnstart) {
      game.id = get_game_id(input_str.data() + i);
      lnstart = false;
      continue;
    }

    if (should_parse_set) {
      set_item_t set;
      parse_set(input_str.data() + i + 1, &set);

      switch (set.type) {
      case 'r':
        game_set.r = set.amount;
        break;
      case 'g':
        game_set.g = set.amount;
        break;
      case 'b':
        game_set.b = set.amount;
      }

      should_parse_set = false;
    }

    bool nlfound = c == '\n', semifound = c == ';';

    if (nlfound || semifound) {
      game.sets.push_back(game_set);
      game_set.r = 0;
      game_set.g = 0;
      game_set.b = 0;

      if (semifound) {
        should_parse_set = true;
      }
    }

    if (nlfound) {
      lnstart = true;

      if (game.id >= 0) {
        games.push_back(game);
        game.id = -1;
        game.sets.clear();
      }

      continue;
    }

    if (c == ',' || c == ':') {
      should_parse_set = true;
    }
  }

#ifdef DEBUG
  std::cerr << "games.size(" << games.size() << ")\n";
  for (const auto &i : games) {
    std::cerr << "gid(" << i.id << ") set_siz(" << i.sets.size() << ")\n";
    for (const auto &j : i.sets) {
      std::cerr << j.r << ' ' << j.g << ' ' << j.b << '\n';
    }
  }
#endif

  int sum = 0;

  for (const auto &game : games) {
    bool impossible = false;
    for (const auto &set : game.sets) {

      if (set.r > bag.r || set.g > bag.g || set.b > bag.b) {
        impossible = true;
        break;
      }
    }

    if (impossible)
      continue;

    sum += game.id;
  }

  std::cout << "Sum: " << sum << '\n';

  // part 2
  int64_t sets_sum = 0;
  for (const auto &game : games) {
    game_set_t cur = {0, 0, 0};
    for (const auto &set : game.sets) {

#ifdef DEBUG
      std::cerr << "\nset.r(" << set.r << ")\n";
      std::cerr << "set.g(" << set.g << ")\n";
      std::cerr << "set.b(" << set.b << ")\n";
#endif

      if (set.r > cur.r) {
        cur.r = set.r;
      }
      if (set.g > cur.g) {
        cur.g = set.g;
      }
      if (set.b > cur.b) {
        cur.b = set.b;
      }
    }

    int64_t pow = cur.r * cur.g * cur.b;

#ifdef DEBUG
    std::cerr << "r(" << cur.r << ") g(" << cur.g << ") b(" << cur.b << ") pow("
              << pow << ")\n";
#endif
    /*
    #ifdef DEBUG
        std::cerr << cur.r << " " << cur.g << " " << cur.b << " " << pow <<
    "\n"; #endif
    */

    sets_sum += pow;
  }

  std::cout << "Sets sum: " << sets_sum << '\n';

  return 0;
}
