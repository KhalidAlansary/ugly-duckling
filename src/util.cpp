#include <cassert>
#include <string>
#include <vector>

#include "parser.hpp"
#include "util.hpp"

static std::vector<std::string> parse_topics(const ElementNode& topics_node) {
  std::vector<std::string> topics;
  topics.reserve(topics_node.children.size());

  for (const Node* topic_node : topics_node.children) {
    if (auto* topic_leaf = dynamic_cast<const LeafNode*>(topic_node)) {
      if (topic_leaf->tag_name == "topic") {
        topics.push_back(topic_leaf->content);
      }
    }
  }

  return topics;
}

static Post parse_post(const ElementNode& post_node) {
  std::string body;
  std::vector<std::string> topics;
  topics.reserve(post_node.children.size());

  for (const Node* post_child : post_node.children) {
    if (auto* post_leaf = dynamic_cast<const LeafNode*>(post_child)) {
      if (post_leaf->tag_name == "body") {
        body = post_leaf->content;
      }
    } else if (auto* post_element =
                   dynamic_cast<const ElementNode*>(post_child)) {
      if (post_element->tag_name == "topics") {
        topics = parse_topics(*post_element);
      }
    }
  }

  return Post(body, topics);
}

static std::vector<Post> parse_posts(const ElementNode& posts_node) {
  std::vector<Post> posts;
  posts.reserve(posts_node.children.size());

  for (const Node* post_node : posts_node.children) {
    if (auto* post_element = dynamic_cast<const ElementNode*>(post_node)) {
      if (post_element->tag_name == "post") {
        posts.push_back(parse_post(*post_element));
      }
    }
  }

  return posts;
}

static std::vector<std::string> parse_followers(
    const ElementNode& followers_node) {
  std::vector<std::string> followers;
  followers.reserve(followers_node.children.size());

  for (const Node* follower_node : followers_node.children) {
    if (const auto* follower_element =
            dynamic_cast<const ElementNode*>(follower_node)) {
      if (follower_element->tag_name == "follower") {
        for (const Node* follower_child : follower_element->children) {
          if (const auto* follower_leaf =
                  dynamic_cast<const LeafNode*>(follower_child)) {
            if (follower_leaf->tag_name == "id") {
              followers.push_back(follower_leaf->content);
            }
          }
        }
      }
    }
  }

  return followers;
}

static User parse_user(const ElementNode& user_node) {
  std::string id, name;
  std::vector<Post> posts;
  std::vector<std::string> followers;
  posts.reserve(user_node.children.size());
  followers.reserve(user_node.children.size());

  for (const Node* user_child : user_node.children) {
    if (const auto* user_leaf = dynamic_cast<const LeafNode*>(user_child)) {
      if (user_leaf->tag_name == "id") {
        id = user_leaf->content;
      } else if (user_leaf->tag_name == "name") {
        name = user_leaf->content;
      }
    } else if (const auto* user_element =
                   dynamic_cast<const ElementNode*>(user_child)) {
      if (user_element->tag_name == "posts") {
        posts = parse_posts(*user_element);
      } else if (user_element->tag_name == "followers") {
        followers = parse_followers(*user_element);
      }
    }
  }

  return User(id, name, posts, followers);
}

std::vector<User> parse_users(const ElementNode& users_node) {
  std::vector<User> users;
  users.reserve(users_node.children.size());
  if (users_node.tag_name != "users") {
    return users;
  }

  for (const Node* user_node : users_node.children) {
    if (const auto* user_element =
            dynamic_cast<const ElementNode*>(user_node)) {
      if (user_element->tag_name == "user") {
        users.push_back(parse_user(*user_element));
      }
    }
  }

  return users;
}
