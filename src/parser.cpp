#include <algorithm>
#include <list>
#include <stdexcept>
#include <string>
#include <tuple>

#include "parser.hpp"

XMLNode::XMLNode(const std::string& tag_name,
                 XMLNode* parent,
                 const std::list<XMLNode>& children,
                 const std::string& content,
                 const std::unordered_map<std::string, std::string>& attributes)
    : tag_name(tag_name),
      parent(parent),
      children(children),
      content(content),
      attributes(attributes) {}

std::string XMLNode::get_opening_tag() const {
  // TODO: Handle attributes
  return "<" + tag_name + ">";
}

std::string XMLNode::get_closing_tag() const {
  return "</" + tag_name + ">";
}

static std::string trim_string(const std::string& str) {
  std::string::size_type start =
      std::find_if(str.cbegin(), str.cend(),
                   [](char ch) { return !std::isspace(ch); }) -
      str.cbegin();
  std::string::size_type end =
      std::find_if(str.crbegin(), str.crend(),
                   [](char ch) { return !std::isspace(ch); }) -
      str.crbegin();
  return str.substr(start, str.size() - start - end);
}

std::tuple<XMLNode, bool> parse_xml(const std::string& xml) {
  if (xml.empty()) {
    return {XMLNode(""), true};
  }

  if (xml[0] != '<' || xml[1] == '/') {
    // Throw an exception if the first tag is a closing tag.
    throw std::invalid_argument("Cannot parse XML");
  }

  XMLNode root = XMLNode("ROOT");
  XMLNode* current_node = &root;
  bool is_valid = true;

  for (std::string::size_type i = 0; i < xml.size(); i++) {
    // Check for opening tag
    if (xml[i] == '<' && xml[i + 1] != '/') {
      // TODO: Parse attributes
      const std::string::size_type tag_close = xml.find('>', i);
      const std::string tag = xml.substr(i + 1, tag_close - i - 1);
      i = tag_close;
      // Add new node to children of current node.
      current_node->children.emplace_back(tag, current_node);
      current_node = &current_node->children.back();
    }
    // Check for closing tag
    else if (xml[i] == '<' && xml[i + 1] == '/') {
      // Assume that the closing tag is the same as the opening tag.
      // This is done to fix misplaced closing tags.
      const std::string::size_type j = xml.find('>', i);
      const std::string tag = xml.substr(i + 2, j - i - 2);
      if (current_node->tag_name != tag) {
        is_valid = false;
      }
      i = j;
      current_node = current_node->parent;
    }
    // Check for content
    else {
      const std::string::size_type content_end = xml.find('<', i);
      const std::string content = trim_string(xml.substr(i, content_end - i));
      i = content_end - 1;
      current_node->content = content;
    }
  }
  return {root, is_valid};
}
