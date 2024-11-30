#include <algorithm>
#include <list>
#include <stdexcept>
#include <string>

#include "parser.hpp"

XMLNode::XMLNode(const std::string& tag_name,
                 XMLNode* parent,
                 const std::unordered_map<std::string, std::string>& attributes,
                 const std::list<XMLNode>& children,
                 const std::string& content)
    : tag_name(tag_name),
      parent(parent),
      attributes(attributes),
      children(children),
      content(content) {}

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

XMLNode parse_xml(const std::string& xml) {
  if (xml.empty()) {
    return XMLNode("");
  }
  if (xml[0] != '<' || xml[1] == '/') {
    // Throw an exception if the first tag is a closing tag.
    throw std::invalid_argument("Cannot parse XML");
  }
  XMLNode root = XMLNode("ROOT");
  XMLNode* current_node = &root;
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
      i = xml.find('>', i);
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
  return root;
}
