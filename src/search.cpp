#include <algorithm>
#include <boost/program_options.hpp>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include "parser.hpp"
#include "search.hpp"
#include "util.hpp"

std::vector<Post> word_search(const std::vector<User>& users,
                              const std::string& keyword) {
  std::vector<Post> matching_posts;

  for (const User& user : users) {
    for (const Post& post : user.posts) {
      if (post.body.find(keyword) != std::string::npos) {
        matching_posts.push_back(post);
      }
    }
  }

  return matching_posts;
}
std::vector<Post> topic_search(const std::vector<User>& users,
                               const std::string& keyword) {
  std::vector<Post> matching_posts;

  for (const User& user : users) {
    for (const Post& post : user.posts) {
      for (const std::string& topic : post.topics) {
        if (topic == keyword) {
          matching_posts.push_back(post);
          break;
        }
      }
    }
  }

  return matching_posts;
}
