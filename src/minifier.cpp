#include <functional>
#include <sstream>
#include <string>

#include "minifier.hpp"
#include "parser.hpp"

std::string minify(const XMLNode& root) {
  const std::function<void(const XMLNode&, std::ostringstream&)> helper =
      [&helper](const XMLNode& node, std::ostringstream& oss) {
        oss << node.get_opening_tag();
        oss << node.content;
        for (const XMLNode& child : node.children) {
          helper(child, oss);
        }
        oss << node.get_closing_tag();
      };

  std::ostringstream oss;
  for (const XMLNode& child : root.children) {
    helper(child, oss);
  }
  return oss.str();
}
