#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include "parser.hpp"
#include <vector>
/**
 * @brief Compress an XML tree to a compressed string.
 *
 * @param root The root of the XML tree.
 * @return The compressed string.
 */
std::string compress(const ElementNode& root);

/**
 * @brief Command-line compression function.
 *
 * Compresses an XML file specified via command-line arguments and saves the
 * compressed result in the specified output file.
 *
 * @param argc The argument count.
 * @param argv The argument vector.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int compress(int argc, char* argv[]);

#endif
