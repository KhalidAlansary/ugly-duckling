#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>

/**
 * @brief Constructs an XMLNode.
 *
 * @param tag_name The name of the tag (required).
 * @param parent The parent node (optional).
 * @param children The list of child nodes (optional).
 * @param content The content of the node (optional).
 * @param attributes The attributes of the node (optional).
 */
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

  /**
   * @brief Returns the opening tag in angle brackets.
   *
   * For example, if the tag name is "h", it returns "<h>".
   *
   * @return The opening tag as a string.
   */
  std::string get_opening_tag() const;

  /**
   * @brief Returns the closing tag in angle brackets.
   *
   * For example, if the tag name is "h", it returns "</h>".
   *
   * @return The closing tag as a string.
   */
  std::string get_closing_tag() const;
};

/**
 * @brief Parses an XML string into an XMLNode tree.
 *
 * @param xml The XML string to parse.
 * @return A tuple containing the dummy root XMLNode which has the rest of the
 * XML as children and a boolean indicating whether the original string was
 * valid.
 */
std::tuple<XMLNode, bool> parse_xml(const std::string& xml);

#endif
