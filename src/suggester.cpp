#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include "parser.hpp"
#include "suggester.hpp"
#include "util.hpp"

std::string suggest(const std::vector<User>& users,
                    const std::string& user_id) {
  std::stringstream result;
  bool found_suggestions = false;
  std::unordered_map<std::string, std::set<std::string>> followers;
  for (const User& user : users) {
    for (const std::string& follower : user.followers) {
      followers[follower].insert(user.id);
    }
  }
  if (followers.contains(user_id)) {
    for (const std::string& follower : followers[user_id]) {
      if (followers.contains(follower)) {
        for (const std::string& follower_of_follower : followers[follower]) {
          if (follower_of_follower != user_id &&
              !followers[user_id].contains(follower_of_follower)) {
            if (found_suggestions) {
              result << ", ";
            }
            result << follower_of_follower;
            found_suggestions = true;
          }
        }
      }
    }
  }

  return found_suggestions ? result.str() : "No suggestions available.";
}