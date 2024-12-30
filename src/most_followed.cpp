#include <string>
#include <unordered_map>
#include <vector>

#include "most_followed.hpp"
#include "util.hpp"

User most_followed(std::vector<User>& users) {
  size_t max_followers = 0;
  User const* most_followed_user = nullptr;
  for (const User& user : users) {
    if (user.followers.size() > max_followers) {
      max_followers = user.followers.size();
      most_followed_user = &user;
    }
  }
  return *most_followed_user;
}
