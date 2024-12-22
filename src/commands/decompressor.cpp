
#include "decompressor.hpp"
#include <unordered_map>
#include <sstream>
#include "parser.hpp"  // Assuming parse_xml is declared here
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
namespace po = boost::program_options;


/**
 * Expands the compressed XML string by replacing short tags with the original ones.
 * @param compressed_xml The compressed XML string.
 * @return The expanded XML string with full tag names.
 */
std::string expand_tags(const std::string& compressed_xml) {
    // Define two parallel vectors: one for compressed tags and one for original tags
    std::vector<std::string> compressed_tags = {
        "ps", "pt", "bd", "tp", "tpc",
        "flwrs", "flwr", "i", "nm", "usr"
    };

    std::vector<std::string> original_tags = {
        "posts", "post", "body", "topics", "topic",
        "followers", "follower", "id", "name", "user"
    };

    std::string expanded_xml = compressed_xml;

    // Loop through each compressed tag and replace it with the corresponding original tag
    for (size_t i = 0; i < compressed_tags.size(); ++i) {
        size_t pos = 0;
        // Replace opening tags
        while ((pos = expanded_xml.find("<" + compressed_tags[i] + ">", pos)) != std::string::npos) {
            expanded_xml.replace(pos, compressed_tags[i].length() + 2, "<" + original_tags[i] + ">");
            pos += original_tags[i].length() + 2; // Move past the replaced tag
        }

        pos = 0;
        // Replace closing tags
        while ((pos = expanded_xml.find("</" + compressed_tags[i] + ">", pos)) != std::string::npos) {
            expanded_xml.replace(pos, compressed_tags[i].length() + 3, "</" + original_tags[i] + ">");
            pos += original_tags[i].length() + 3;
        }
    }

    return expanded_xml;
}
/**
 * Decompress the compressed XML string by restoring the full tag names and parsing it.
 * @param compressed_xml The compressed XML string.
 * @return The XML tree as a tuple (root node, valid flag).
 */
std::tuple<ElementNode, bool> decompressor(const std::string& compressed_xml) {
    // Step 1: Expand the short tags back to full tags
    std::string expanded_xml = expand_tags(compressed_xml);

    // Step 2: Parse the expanded XML string into an XML tree
    return parse_xml(expanded_xml);
}



int decompress(int argc, char* argv[]) {
    try {
        std::string input_file;
        std::string output_file;

        // Define command-line options
        po::options_description desc("Decompress options");
        desc.add_options()
            ("help", "Show help message")
            ("input,i", po::value<std::string>(&input_file)->required(), "Input compressed file")
            ("output,o", po::value<std::string>(&output_file)->required(), "Output decompressed XML file");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        po::notify(vm);

        // Open the input compressed file
        std::ifstream input(input_file);
        if (!input.is_open()) {
            std::cerr << "Error: Could not open input file\n";
            return EXIT_FAILURE;
        }

        // Read the compressed content
        std::string compressed((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        input.close();

        // Generate the decompressed string
        std::string decompressed = decompress(compressed);

        // Open the output file
        std::ofstream output(output_file);
        if (!output.is_open()) {
            std::cerr << "Error: Could not open output file\n";
            return EXIT_FAILURE;
        }

        // Write the decompressed string to the output file
        output << decompressed;
        output.close();

        std::cout << "Decompression successful. Output saved to " << output_file << "\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
