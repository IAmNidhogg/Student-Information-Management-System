#ifndef PARSING_HEADER
#define PARSING_HEADER

#include "myjson.hpp"

JSON parseObject(const std::string &s, int begin, int end,
                 std::unordered_map<int, int> &bracepair);
JSON parseArray(const std::string &s, int begin, int end,
                std::unordered_map<int, int> &bracepair);
JSON parse(const std::string &s);

#endif
