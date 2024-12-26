#include <functional>
#include <sstream>
#include <string>

#include "converter.hpp"
#include "parser.hpp"

std::string convert(const ElementNode& root) {
  std::function<void(const Node* const, int, std::ostringstream&)> helper =
      [&helper](const Node* const node, int depth,
                std::ostringstream& oss) -> void {
    // Leaf Node
    if (auto leaf = dynamic_cast<const LeafNode*>(node)) {
      oss << std::string(depth * INDENTATION_SIZE, INDENTATION_CHAR) << '\"'
          << leaf->tag_name << "\": \"" << leaf->content << '\"';
    }
    // Element Node
    else if (auto element = dynamic_cast<const ElementNode*>(node)) {
      oss << std::string(depth * INDENTATION_SIZE, INDENTATION_CHAR) << '\"'
          << element->tag_name << "\": ";
      if (element->tag_name.back() == 's') {
        oss << '[';
        for (const auto& child : element->children) {
          oss << '\n'
              << std::string((depth + 1) * INDENTATION_SIZE, INDENTATION_CHAR)
              << "{\n";
          helper(child, depth + 2, oss);
          oss << '\n'
              << std::string((depth + 1) * INDENTATION_SIZE, INDENTATION_CHAR)
              << '}';
          if (child != element->children.back()) {
            oss << ',';
          }
        }
        oss << '\n'
            << std::string(depth * INDENTATION_SIZE, INDENTATION_CHAR) << "]";
      } else {
        oss << '{';
        for (const auto& child : element->children) {
          oss << "\n";
          helper(child, depth + 1, oss);
          if (child != element->children.back()) {
            oss << ',';
          }
        }
        oss << "\n"
            << std::string(depth * INDENTATION_SIZE, INDENTATION_CHAR) << "}";
        if (node != element->parent->children.back() &&
            element->parent->tag_name.back() != 's') {
          oss << ',';
        }
      }
    }
  };

  std::ostringstream oss;
  // Assuming there is one root element
  oss << "{\n";
  helper(root.children.front(), 1, oss);
  oss << "\n}\n";
  return oss.str();
}
