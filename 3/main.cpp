#include "main.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

constexpr const char *default_inpfile = "input.txt";

bool is_digit(char c) { return (c >= '0') && (c <= '9'); }

bool has_pos(const std::vector<num_pos_t> &pos_list, const num_pos_t &npt) {
  for (const auto &i : pos_list) {
    if (i.line_idx == npt.line_idx && i.first_idx == npt.first_idx &&
        i.last_idx == npt.last_idx)
      return true;
  }

  return false;
}

void store_pos(std::vector<num_pos_t> &pos_list, const num_pos_t &npt) {
  if (has_pos(pos_list, npt))
    return;

  pos_list.push_back(npt);
}

size_t get_number_from_num_pos_t(const std::vector<std::string> &lines,
                                 const num_pos_t &npt,
                                 const char *dbg_pos = "get_number_from_part") {
  const std::string &line = lines.at(npt.line_idx);
  std::string num = line.substr(npt.first_idx, npt.last_idx - npt.first_idx);

  return stoul(num);
}

// this algorithm is absolute and will always work,
// if not then it's ur fault for providing wrong/invalid argument!
// the line_idx of the return value must by set by the caller
num_pos_t get_num_pos_t_from_part(size_t initial_idx, const std::string &line) {
  // seek left and right for the whole number
  size_t idx_first = initial_idx;
  size_t idx_last = initial_idx;
  constexpr size_t min_x = 0;
  size_t max_x = line.length() - 1;

  bool hit0 = false;
  while ((--idx_first) >= min_x && is_digit(line.at(idx_first))) {
    if (idx_first == 0) {
      hit0 = true;
      break;
    }
  }

  if (!hit0)
    idx_first++;

  while ((++idx_last) <= max_x && is_digit(line.at(idx_last)))
    ;

  return {0, idx_first, idx_last};
}

int main(const int argc, const char *argv[]) {
  const char *inpfile = (argc > 1) ? argv[1] : default_inpfile;

  FILE *input = fopen(inpfile, "r");
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

  // list all number first
  std::vector<int> listn;
  std::vector<std::string> lines;

  char ntemp[10];
  size_t ntemp_siz = 0;
  std::string ln;
  ln.reserve(input_len);
  for (char c : input_str) {
    if (ntemp_siz == 10)
      return 1;

    bool is_nl = c == '\n';

    if (is_nl) {
      lines.push_back(ln);
      ln.clear();
    } else
      ln += c;

    if (is_digit(c)) {
      ntemp[ntemp_siz++] = c;
      continue;
    }

    // not digit VVV
    if (ntemp_siz > 0) {
      ntemp[ntemp_siz] = '\0';

      listn.push_back(atoi(ntemp));

      ntemp_siz = 0;
    }
  }

  if (ntemp_siz > 0) {
    ntemp[ntemp_siz] = '\0';

    listn.push_back(atoi(ntemp));

    ntemp_siz = 0;
  }

#ifdef DEBUG
  for (int i : listn) {
    std::cerr << i << ' ';
  }
  std::cerr << '\n';
#endif

  // find symbols, or whatever not dot and digit
  // and look for adjacent digits
  std::vector<num_pos_t> pos_list;
  size_t lines_len = lines.size();

  constexpr size_t min_y = 0;
  size_t max_y = lines_len - 1;

  // i == y idx
  for (size_t i = 0; i < lines_len; i++) {
    const std::string &line = lines.at(i);
    size_t line_len = line.length();

    size_t top_line_idx = i - 1;
    size_t bottom_line_idx = i + 1;

    // j == x idx
    for (size_t j = 0; j < line_len; j++) {
      char lc = line.at(j);
      size_t left_idx = j - 1;
      size_t right_idx = j + 1;

      // skip digit, dot, newline
      if (is_digit(lc) || lc == '.' || lc == '\n')
        continue;

      // top left   ,  top ,   top right
      // left       ,   X  ,       right
      // bottom left,bottom,bottom right

      if (i > min_y) {
        // check top size
        const std::string &top_line = lines.at(top_line_idx);
        constexpr size_t min_x_top = 0;
        size_t max_x_top = top_line.length() - 1;

        // left
        if (j > min_x_top) {
          char part = top_line[left_idx];
          if (is_digit(part)) {
            num_pos_t npt = get_num_pos_t_from_part(left_idx, top_line);
            npt.line_idx = top_line_idx;

            store_pos(pos_list, npt);
          }
        }

        // top
        char part = top_line[j];
        if (is_digit(part)) {
          num_pos_t npt = get_num_pos_t_from_part(j, top_line);
          npt.line_idx = top_line_idx;

          store_pos(pos_list, npt);
        }

        // right
        if (j < max_x_top) {
          char part = top_line[right_idx];
          if (is_digit(part)) {
            num_pos_t npt = get_num_pos_t_from_part(right_idx, top_line);
            npt.line_idx = top_line_idx;

            store_pos(pos_list, npt);
          }
        }
      }

      constexpr size_t min_x = 0;
      size_t max_x = line_len - 1;

      // check left
      if (j > min_x) {
        char part = line[left_idx];
        if (is_digit(part)) {
          num_pos_t npt = get_num_pos_t_from_part(left_idx, line);
          npt.line_idx = i;

          store_pos(pos_list, npt);
        }
      }

      // and right
      if (j < max_x) {
        char part = line[right_idx];
        if (is_digit(part)) {
          num_pos_t npt = get_num_pos_t_from_part(right_idx, line);
          npt.line_idx = i;

          store_pos(pos_list, npt);
        }
      }

      if (i < max_y) {
        // check bottom side
        const std::string &bottom_line = lines.at(bottom_line_idx);
        constexpr size_t min_x_bottom = 0;
        size_t max_x_bottom = bottom_line.length() - 1;

        // left
        if (j > min_x_bottom) {
          char part = bottom_line[left_idx];
          if (is_digit(part)) {
            num_pos_t npt = get_num_pos_t_from_part(left_idx, bottom_line);
            npt.line_idx = bottom_line_idx;

            store_pos(pos_list, npt);
          }
        }

        // bottom
        char part = bottom_line[j];
        if (is_digit(part)) {
          num_pos_t npt = get_num_pos_t_from_part(j, bottom_line);
          npt.line_idx = bottom_line_idx;

          store_pos(pos_list, npt);
        }

        // right
        if (j < max_x_bottom) {
          char part = bottom_line[right_idx];
          if (is_digit(part)) {
            num_pos_t npt = get_num_pos_t_from_part(right_idx, bottom_line);
            npt.line_idx = bottom_line_idx;

            store_pos(pos_list, npt);
          }
        }
      }
    }
  }

  size_t sum = 0;

  for (const num_pos_t &p : pos_list) {
    size_t n = get_number_from_num_pos_t(lines, p);
    sum += n;
#ifdef DEBUG
    std::cerr << "line(" << p.line_idx << ") first(" << p.first_idx << ") last("
              << p.last_idx << ") val(" << n << ")\n";
#endif
  }

  std::cout << "\nSum: " << sum << "\n\n";

  return 0;
}
