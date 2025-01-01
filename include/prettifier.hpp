#ifndef PRETTIFIER_HPP
#define PRETTIFIER_HPP

#include <string>

#include "parser.hpp"

#define XML_INDENTATION_SIZE 4
#define XML_MAX_LINE_LENGTH 80
#define XML_INDENTATION_CHAR ' '

/**
 * Get the formatted string representation of an XML tree.
 * @param root The dummy root node of the XML tree.
 * @return The prettified XML tree.
 */
std::string prettify(const ElementNode& root);

#endif
