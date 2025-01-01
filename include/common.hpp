#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>

#include "util.hpp"

std::unordered_set<std::string> common(const std::vector<User>& users,
                                       const std::vector<std::string>& ids);

#endif
