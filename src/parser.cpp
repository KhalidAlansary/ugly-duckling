#include <algorithm>
#include <list>
#include <string>
#include <tuple>

#include "parser.hpp"

// WARNING: Memory leak
// Use smart pointers to avoid memory leaks or create a function te free the
// memory

Node::Node(ElementNode* parent) : parent(parent) {}

XMLNode::XMLNode(const std::string& tag_name, ElementNode* parent)
    : Node(parent), tag_name(tag_name) {}

ElementNode::ElementNode(const std::string& tag_name,
                         ElementNode* parent,
                         const std::list<Node*>& children)
    : XMLNode(tag_name, parent), children(children) {}

LeafNode::LeafNode(const std::string& tag_name,
                   ElementNode* parent,
                   const std::string& content)
    : XMLNode(tag_name, parent), content(content) {}

CommentNode::CommentNode(const std::string& comment, ElementNode* parent)
    : Node(parent), comment(comment) {}

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

std::tuple<ElementNode, bool> parse_xml(const std::string& xml) {
  ElementNode root("ROOT", nullptr);
  ElementNode* current = &root;
  bool is_valid = true;
  std::string::size_type i = xml.find_first_of('<');
  while (i != std::string::npos) {
    // Comment
    if (xml.substr(i + 1, 3) == "!--") {
      const std::string::size_type comment_end = xml.find("-->", i + 4);
      if (comment_end == std::string::npos) {
        is_valid = false;
      }
      const std::string comment =
          trim_string(xml.substr(i + 4, comment_end - i - 4));
      current->children.push_back(new CommentNode(comment, current));
      i = comment_end + 3;
    }
    // Closing tag
    else if (xml[i + 1] == '/') {
      const std::string::size_type tag_end = xml.find('>', i + 2);
      if (tag_end == std::string::npos) {
        is_valid = false;
      }
      const std::string tag_name =
          trim_string(xml.substr(i + 2, tag_end - i - 2));
      if (tag_name != current->tag_name) {
        is_valid = false;
        current->children.push_back(new LeafNode(tag_name, current));
      }
      current = current->parent;
      i = tag_end + 1;
    }
    // Opening tag
    else {
      const std::string::size_type tag_end = xml.find('>', i + 1);
      if (tag_end == std::string::npos) {
        is_valid = false;
      }
      const std::string tag_name =
          trim_string(xml.substr(i + 1, tag_end - i - 1));
      const std::string::size_type content_end = xml.find('<', tag_end + 1) - 1;
      if (content_end == std::string::npos) {
        is_valid = false;
      }
      const std::string content =
          trim_string(xml.substr(tag_end + 1, content_end - tag_end));
      if (content.empty()) {
        ElementNode* new_element = new ElementNode(tag_name, current);
        current->children.push_back(new_element);
        current = new_element;
        i = content_end + 1;
      } else {
        current->children.push_back(new LeafNode(tag_name, current, content));
        if (xml[content_end + 2] == '/') {
          const std::string::size_type closing_tag_end =
              xml.find('>', content_end + 2);
          const std::string closing_tag_name = trim_string(
              xml.substr(content_end + 3, closing_tag_end - content_end - 3));
          if (closing_tag_name != tag_name) {
            is_valid = false;
            current->children.push_back(
                new LeafNode(closing_tag_name, current));
          }
          i = closing_tag_end + 1;
        } else {
          is_valid = false;
          i = content_end + 1;
        }
      }
    }
    i = xml.find_first_of('<', i);
  }

  if (root.children.size() != 1) {
    is_valid = false;
  }
  return {root, is_valid};
}
