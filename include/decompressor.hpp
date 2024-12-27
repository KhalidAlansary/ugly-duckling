#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <string>

#include "parser.hpp"

/**
 * @brief Decompresses the input XML string.
 *
 * This function parses the compressed XML string into a tree, decompresses the
 * tags, and then minifies the resulting tree back into a string.
 *
 * @param xml The XML string to be decompressed.
 * @return A decompressed XML string.
 */

std::string decompress_xml(const std::string& xml);

/**
 * @brief decompresses the XML tree in place.
 *
 * This function modifies the provided tree by replacing compresssed tag names
 * with decompressed versions.
 *
 * @param root The root of the ElementNode tree.
 */

void decompress_tree_in_place(ElementNode& root);

/**
 * @brief Main function to compress the XML from command-line arguments.
 *
 * This function reads the compressed XML file specified in the command line
 * arguments, decompresses it, and prints the decompressed result.
 */

int decompress(int argc, char* argv[]);

#endif
