#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include "parser.hpp"
#include <vector>
#include <functional>
#include <sstream>
#include "minifier.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
namespace po = boost::program_options;

/**
 * @brief Compresses the input XML string.
 *
 * This function parses the XML string into a tree, compresses the tags, 
 * and then minifies the resulting tree back into a string.
 *
 * @param xml The XML string to be compressed.
 * @return A compressed XML string.
 */

std::string compress_xml(const std::string& xml);

/**
 * @brief Compresses the XML tree in place.
 *
 * This function modifies the provided tree by replacing tag names with compressed versions.
 *
 * @param root The root of the ElementNode tree.
 */

void compress_tree_in_place(ElementNode& root);

/**
 * @brief Main function to compress the XML from command-line arguments.
 *
 * This function reads the XML file specified in the command line arguments,
 * compresses it, and prints the compressed result.
 */

int compress(int argc, char* argv[]);

#endif
