#include <functional>
#include <sstream>
#include <string>

#include "minifier.hpp"
#include "parser.hpp"

std::string minify(const ElementNode& root) {
  std::function<void(const ElementNode&, std::ostringstream&)> helper =
      [&helper](const ElementNode& node, std::ostringstream& oss) -> void {
    oss << node.get_opening_tag();
    for (Node* child : node.children) {
      // LeafNode
      if (LeafNode* leaf = dynamic_cast<LeafNode*>(child)) {
        oss << leaf->get_opening_tag() << leaf->content
            << leaf->get_closing_tag();
      }
      // ElementNode
      else if (ElementNode* element = dynamic_cast<ElementNode*>(child)) {
        helper(*element, oss);
      }
    }
    oss << node.get_closing_tag();
  };

  std::ostringstream oss;
  helper(*dynamic_cast<ElementNode*>(root.children.front()), oss);
  return oss.str();
}
