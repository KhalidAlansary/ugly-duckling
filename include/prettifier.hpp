#ifndef PRETTIFIER_HPP
#define PRETTIFIER_HPP

#include <string>

#include "parser.hpp"

#ifndef INDENTATION_SIZE
#define INDENTATION_SIZE 4
#endif
#define MAX_LINE_LENGTH 80
#define INDENTATION_CHAR ' '

/**
 * Get the formatted string representation of an XML tree.
 * @param root The dummy root node of the XML tree.
 * @return The prettified XML tree.
 */
std::string prettify(const ElementNode& root);

#endif
