#include <string>
#include <unordered_set>
#include <vector>

#include "suggester.hpp"
#include "util.hpp"

std::unordered_set<std::string> suggest(const std::vector<User>& users,
                                        const std::string& user_id) {
  std::unordered_set<std::string> suggestions;
  const std::unordered_set<std::string>* my_followers;
  for (const User& user : users) {
    if (user.id == user_id) {
      my_followers = &user.followers;
      break;
    }
  }
  for (const User& user : users) {
    if (my_followers->contains(user.id)) {
      suggestions.insert(user.followers.begin(), user.followers.end());
      for (const std::string& folower_id : user.followers) {
        if (folower_id == user_id) {
          suggestions.erase(folower_id);
        }
      }
    }
  }
  suggestions.erase(user_id);
  return suggestions.empty()
             ? std::unordered_set<
                   std::string>{"No suggestions available for this user"}
             : suggestions;
}