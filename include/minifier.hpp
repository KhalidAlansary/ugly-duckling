#ifndef MINIFIER_HPP
#define MINIFIER_HPP

#include <string>

#include "parser.hpp"

/**
 * Get the minified string representation of an XML tree without spaces or
 * newlines.
 * @param root The dummy root node of the XML tree.
 * @return The minified XML tree without spaces or newlines.
 */
std::string minify(const XMLNode& root);

#endif
