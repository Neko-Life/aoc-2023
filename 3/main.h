#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>

struct num_pos_t {
  size_t line_idx;
  size_t first_idx;
  size_t last_idx;
  char sym;
  size_t sym_line_idx;
  size_t sym_x_idx;
};

#endif // MAIN_H
