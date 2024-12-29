#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <unordered_set>
#include <vector>

#include "parser.hpp"

class Post {
 public:
  const std::string body;
  std::vector<std::string> topics;
  Post(const std::string& body, const std::vector<std::string>& topics)
      : body(body), topics(topics) {}
};

class User {
 public:
  const std::string id, name;
  const std::vector<Post> posts;
  const std::unordered_set<std::string> followers;
  User(const std::string& id,
       const std::string& name,
       const std::vector<Post>& posts,
       const std::unordered_set<std::string>& followers)
      : id(id), name(name), posts(posts), followers(followers) {}
};

/**
 * @brief Parse a list of users from an ElementNode.
 *
 * @param users_node The ElementNode to parse.
 * @return std::vector<User> The list of users.
 */
std::vector<User> parse_users(const ElementNode& users_node);

#endif
