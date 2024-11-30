#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include <unordered_map>

// Represents an XML node.
class XMLNode {
 public:
  const std::string tag_name;
  XMLNode* const parent;
  std::list<XMLNode> children;
  std::string content;
  const std::unordered_map<std::string, std::string> attributes;

  XMLNode(const std::string& tag_name,
          XMLNode* parent = nullptr,
          const std::list<XMLNode>& children = {},
          const std::string& content = "",
          const std::unordered_map<std::string, std::string>& attributes = {});
};

XMLNode parse_xml(const std::string& xml);

#endif
