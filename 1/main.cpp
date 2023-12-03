#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

constexpr char digits[] = "0987654321";
constexpr char *str_digits[] = {"one", "two",   "three", "four", "five",
                                "six", "seven", "eight", "nine", NULL};

bool contain_digit(const char *s1, const char *s2) {
  for (int i = 0; s1[i] != (char)NULL; i++) {
    if (s2[i] == (char)NULL)
      return true;

    if (s1[i] != s2[i])
      return false;
  }

  return false;
}

char str_digit_to_digit(const char *str) {
  int c = 0;

  for (int a = 0; a < (sizeof(str_digits) / sizeof(*str_digits)); a++) {
    const char *ptr = str_digits[a];
    if (!ptr)
      break;

    if (contain_digit(str, ptr)) {
      c = a + 1;
      break;
    }
  }

  if (c == 0)
    return 0;

  return c + '0';
}

bool is_digit(char c) {
  for (char comp : digits) {
    if (comp == c)
      return true;
  }

  return false;
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

  char *buf = (char *)malloc(sizeof(char) * fsiz + 1);
  ssize_t readsiz = fread(buf, 1, fsiz, input);
  buf[readsiz] = '\0';
  fclose(input);

  std::string input_str(buf);
  free(buf);

  size_t input_len = input_str.length();

  std::vector<std::string> nlist;
  std::string temp;

#ifdef DEBUG
  std::string temp_input;

  std::cerr << input_str << "\n\n";
#endif

  for (size_t i = 0; i < input_len; i++) {
    char c = input_str.at(i);

    if (c == '\n') {
      char c[3];

      if (!temp.empty()) {
        size_t len = temp.length();
        bool has_2 = len > 1;

        c[0] = temp.at(0);
        c[1] = has_2 ? temp.at(len - 1) : c[0];
        c[2] = '\0';

        nlist.push_back(c);
      }

#ifdef DEBUG
      std::cerr << "Got nl: " << temp_input << ": " << temp << " : " << c
                << "\n";
      temp_input.clear();
#endif

      temp.clear();
      continue;
    }

    char cn;

    if (is_digit(c)) {
      temp += c;
    } else if ((cn = str_digit_to_digit(input_str.data() + i)) &&
               is_digit(cn)) {
      temp += cn;
    }
#ifdef DEBUG
    temp_input += c;
#endif
  }

  if (!temp.empty()) {
    char c[3];
    size_t len = temp.length();
    bool has_2 = len > 1;

    c[0] = temp.at(0);
    c[1] = has_2 ? temp.at(len - 1) : c[0];
    c[2] = '\0';

    nlist.push_back(c);

#ifdef DEBUG
    std::cerr << "Final: " << temp_input << ": " << temp << "\n";
    temp_input.clear();
#endif

    temp.clear();
  }

  int64_t sum = 0;
  for (const std::string &s : nlist) {
    sum += stoll(s);
  }

  std::cout << "\nSum: " << sum << "\n\n";

  return 0;
}
