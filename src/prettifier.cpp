#include <functional>
#include <sstream>
#include <string>

#include "parser.hpp"
#include "prettifier.hpp"

std::string prettify(const ElementNode& root) {
  std::function<void(const ElementNode&, int, std::ostringstream&)> helper =
      [&helper](const ElementNode& node, int depth,
                std::ostringstream& oss) -> void {
    oss << std::string(XML_INDENTATION_SIZE * depth, XML_INDENTATION_CHAR);
    oss << node.get_opening_tag() << '\n';
    for (Node* child : node.children) {
      // LeafNode
      if (LeafNode* leaf = dynamic_cast<LeafNode*>(child)) {
        oss << std::string(XML_INDENTATION_SIZE * (depth + 1),
                           XML_INDENTATION_CHAR)
            << leaf->get_opening_tag();
        if (leaf->content.size() > XML_MAX_LINE_LENGTH) {
          oss << '\n'
              << std::string(XML_INDENTATION_SIZE * (depth + 2),
                             XML_INDENTATION_CHAR)
              << leaf->content << '\n'
              << std::string(XML_INDENTATION_SIZE * (depth + 1),
                             XML_INDENTATION_CHAR);
        } else {
          oss << leaf->content;
        }
        oss << leaf->get_closing_tag();
      }
      // ElementNode
      else if (ElementNode* element = dynamic_cast<ElementNode*>(child)) {
        helper(*element, depth + 1, oss);
      }
      // CommentNode
      else if (CommentNode* comment = dynamic_cast<CommentNode*>(child)) {
        oss << std::string(XML_INDENTATION_SIZE * (depth + 1),
                           XML_INDENTATION_CHAR)
            << "<!-- " << comment->comment << " -->";
      } else {
        throw std::runtime_error("Unknown node type");
      }
      oss << '\n';
    }
    oss << std::string(XML_INDENTATION_SIZE * depth, XML_INDENTATION_CHAR);
    oss << node.get_closing_tag();
  };

  std::ostringstream oss;
  helper(*dynamic_cast<ElementNode*>(root.children.front()), 0, oss);
  return oss.str();
}
