#ifndef Search_hpp
#define Search_hpp

#include <string>
#include <vector>

#include "parser.hpp"
#include "util.hpp"

std::vector<Post> word_search(const std::vector<User>& users,
                              const std::string& keyword);
std::vector<Post> topic_search(const std::vector<User>& users,
                               const std::string& keyword);

#endif
