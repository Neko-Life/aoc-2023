#include <fstream>
#include <iostream>
#include <vector>

constexpr const char *default_inpfile = "input.txt";

bool is_digit(char c) { return (c >= '0') && (c <= '9'); }

void get_num_list_from_str(std::vector<int> &list,
                           const std::string &list_str) {
  int first_idx = -1;

  size_t llen = list_str.length();

  for (int i = 0; (size_t)i < llen; i++) {
    if (i < 0)
      break;

    char c = list_str.at(i);
#ifdef DEBUG
    std::cerr << "char(" << c << ")\n";
#endif

    bool last_idx = (size_t)i == (llen - 1);
    if ((!is_digit(c) || last_idx) && first_idx >= 0) {
      std::string s =
          list_str.substr(first_idx, (i + (last_idx ? 1 : 0)) - first_idx);
      first_idx = -1;

#ifdef DEBUG
      std::cerr << "got(" << s << ")\n";
#endif

      list.push_back(stoi(s));
      continue;
    }

    if (first_idx < 0) {
      first_idx = i;
    }
  }

  // calculate points
}

int main(const int argc, const char *argv[]) {
  const char *inpfile = (argc > 1) ? argv[1] : default_inpfile;

  std::ifstream input_strm(inpfile);

  size_t total_point = 0;

  // iterate each line
  std::string line;
  while (std::getline(input_strm, line)) {
#ifdef DEBUG
    std::cerr << line << '\n';
#endif

    // get list of winning nums
    size_t idxcolon = line.find(':') + 2;
    size_t idxbar = line.find('|') - 1;
    std::string winlist_str = line.substr(idxcolon, idxbar - idxcolon);

#ifdef DEBUG
    std::cerr << "winlist(" << winlist_str << ")" << '\n';
#endif

    std::vector<int> winlist;
    get_num_list_from_str(winlist, winlist_str);

    size_t idxmybar = idxbar + 3;
    std::string mylist_str = line.substr(idxmybar);

#ifdef DEBUG
    std::cerr << "mylist(" << mylist_str << ")" << '\n';
#endif

    std::vector<int> mylist;
    get_num_list_from_str(mylist, mylist_str);

    size_t curpoint = 0;
    for (int i : mylist) {
      bool win = false;
      for (int j : winlist) {
        if (j != i)
          continue;

        win = true;
        break;
      }

      if (win) {
        if (curpoint == 0) {
          curpoint = 1;
          continue;
        }
        curpoint *= 2;
      }
    }

    total_point += curpoint;
  }

  std::cout << "\nPoint: " << total_point << "\n\n";

  return 0;
}
