#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "parser.hpp"

#define INDENTATION_SIZE 2
#define MAX_LINE_LENGTH 80
#define INDENTATION_CHAR ' '

/**
 * @brief Converts XML represented by tree into json
 *
 * @param root The root ElementNode
 * @return std::string The json representation of the XML
 */
std::string convert(const ElementNode& root);

#endif
