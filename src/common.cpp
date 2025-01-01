#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

#include "util.hpp"

std::unordered_set<std::string> common(const std::vector<User>& users,
                                       const std::vector<std::string>& ids) {
  std::unordered_set<std::string> id_set(ids.begin(), ids.end());
  const std::vector<User>::const_iterator first_user = std::find_if(
      users.begin(), users.end(),
      [&id_set](const User& user) { return id_set.contains(user.id); });

  std::unordered_set<std::string> common_followers = first_user->followers;
  for (std::vector<User>::const_iterator user = first_user + 1;
       user != users.end(); user++) {
    if (!id_set.contains(user->id)) {
      continue;
    }
    std::unordered_set<std::string> new_common_followers;
    for (const std::string& common_follower : common_followers) {
      if (user->followers.contains(common_follower)) {
        new_common_followers.insert(common_follower);
      }
    }
    common_followers = new_common_followers;
  }

  return common_followers;
}
