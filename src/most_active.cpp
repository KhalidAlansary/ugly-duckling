#include <string>
#include <unordered_map>
#include <unordered_set>
#include "util.hpp"

User most_active(const std::vector<User>& users) {
  std::unordered_map<std::string, User const*> user_map;
  for (auto& user : users) {
    user_map[user.id] = &user;
  }

  std::unordered_map<User const*, std::unordered_set<User const*>>
      user_connections;
  for (const User& user : users) {
    for (const std::string& user2 : user.followers) {
      user_connections[&user].insert(user_map[user2]);
      user_connections[user_map[user2]].insert(&user);
    }
  }

  User const* most_active_user = nullptr;
  size_t max_connections = 0;

  for (const auto& user : user_connections) {
    if (user.second.size() > max_connections) {
      max_connections = user.second.size();
      most_active_user = user.first;
    }
  }
  return *most_active_user;
}
