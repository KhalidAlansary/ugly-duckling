#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>
#include <tuple>

class ElementNode;

/**
 * @brief Abstract class representing an XML node.
 *
 * @param parent The parent node of this node.
 */
class Node {
 public:
  ElementNode* parent;

  /**
   * @brief Constructs a new Node.
   *
   * @param parent The parent node of this node.
   */
  explicit Node(ElementNode* parent);
  virtual ~Node() = default;
};

/**
 * @brief Abstract class representing an XML node with a tag name.
 *
 * @param tag_name The tag name of the node.
 * @param parent The parent node of this node.
 */
class XMLNode : public Node {
 public:
  const std::string tag_name;

  /**
   * @brief Constructs a new XMLNode.
   *
   * @param tag_name The tag name of the node.
   * @param parent The parent node of this node.
   */
  XMLNode(const std::string& tag_name, ElementNode* parent);

  /**
   * @brief Returns the opening tag in angle brackets.
   *
   * For example, if the tag name is "h", it returns "<h>".
   *
   * @return The opening tag as a string.
   */
  inline std::string get_opening_tag() const { return "<" + tag_name + ">"; }

  /**
   * @brief Returns the closing tag in angle brackets.
   *
   * For example, if the tag name is "h", it returns "</h>".
   *
   * @return The closing tag as a string.
   */
  inline std::string get_closing_tag() const { return "</" + tag_name + ">"; }
};

/**
 * @brief Class representing an XML element.
 *
 * @param children The children of the element.
 */
class ElementNode : public XMLNode {
 public:
  std::list<Node*> children;

  /**
   * @brief Constructs a new Element.
   *
   * @param tag_name The tag name of the element.
   * @param parent The parent node of this node.
   * @param children The children of the element.
   */
  ElementNode(const std::string& tag_name,
              ElementNode* parent,
              const std::list<Node*>& children = {});
};

/**
 * @brief Class representing an XML leaf.
 *
 * @param content The content of the leaf.
 */
class LeafNode : public XMLNode {
 public:
  const std::string content;

  /**
   * @brief Constructs a new Leaf.
   *
   * @param tag_name The tag name of the leaf.
   * @param parent The parent node of this node.
   * @param content The content of the leaf.
   */
  LeafNode(const std::string& tag_name,
           ElementNode* parent,
           const std::string& content = "");
};

/**
 * @brief Class representing an XML comment.
 *
 * @param comment The content of the comment.
 * @param parent The parent node of this node.
 */
class CommentNode : public Node {
 public:
  const std::string comment;

  /**
   * @brief Constructs a new Comment.
   *
   * @param parent The parent node of this node.
   * @param content The content of the comment.
   */
  CommentNode(const std::string& comment, ElementNode* parent = nullptr);
};

/**
 * @brief Parses an XML string into a tree.
 *
 * @param xml The XML string to parse.
 * @return A tuple containing the dummy root node which has the rest of the
 * XML as children and a boolean indicating whether the original string was
 * valid.
 */
std::tuple<ElementNode, bool> parse_xml(const std::string& xml);

#endif
