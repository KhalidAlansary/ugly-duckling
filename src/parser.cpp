#include <algorithm>
#include <list>
#include <string>
#include <tuple>

#include "parser.hpp"

XMLNode::XMLNode(const std::string& tag_name,
                 XMLNode* parent,
                 const std::list<XMLNode>& children,
                 const std::string& content)
    : tag_name(tag_name),
      parent(parent),
      children(children),
      content(content) {}

std::string XMLNode::get_opening_tag() const {
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
  XMLNode root = XMLNode("ROOT");
  XMLNode* current_node = &root;
  bool is_valid = true;

  std::string::size_type i = 0;
  while (i < xml.size()) {
    // Check for comment
    if (xml[i] == '<' && xml[i + 1] == '!' && xml[i + 2] == '-' &&
        xml[i + 3] == '-') {
      // TODO: Handle comments
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
      i = j + 1;
      current_node = current_node->parent;
    }
    // Check for opening tag
    else if (xml[i] == '<') {
      // If the current tag has content, then there is a missing closing tag.
      if (!current_node->content.empty()) {
        is_valid = false;
        current_node = current_node->parent;
      }
      const std::string::size_type tag_close = xml.find('>', i);
      const std::string tag = xml.substr(i + 1, tag_close - i - 1);
      i = tag_close + 1;
      // Add new node to children of current node.
      current_node->children.emplace_back(tag, current_node);
      current_node = &current_node->children.back();
    }
    // Check for content
    else {
      const std::string::size_type content_end = xml.find('<', i);
      const std::string content = trim_string(xml.substr(i, content_end - i));
      i = content_end;
      current_node->content = content;
    }
  }

  if (root.children.size() > 1) {
    is_valid = false;
  }
  return {root, is_valid};
}
