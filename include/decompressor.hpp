#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <string>
#include "parser.hpp"
#include <vector>
/**
 * @brief Decompress a compressed string to a decompressed string.
 *
 * @param compressed The compressed string.
 * @return The decompressed string.
 */
std::tuple<ElementNode, bool> decompress(const std::string& compressed);

/**
 * @brief Command-line decompression function.
 *
 * Decompresses a compressed file specified via command-line arguments and saves
 * the decompressed XML in the specified output file.
 *
 * @param argc The argument count.
 * @param argv The argument vector.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int decompress(int argc, char* argv[]);

#endif
