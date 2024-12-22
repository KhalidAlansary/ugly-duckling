#include "compressor.hpp"
#include <unordered_map>
#include <sstream>
#include "minifier.hpp"  
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
namespace po = boost::program_options;

/**
 * Compresses the minified XML string by replacing tag names with shorter ones.
 * @param minified_xml The minified XML string.
 * @return The compressed XML string with shorter tags.
 */
std::string compress_tags(const std::string& minified_xml) {


	std::vector<std::string> original_tags = {
	        "posts", "post", "body", "topics", "topic",
	        "followers", "follower", "id", "name", "user"
	    };

	    std::vector<std::string> compressed_tags = {
	        "ps", "pt", "bd", "tp", "tpc",
	        "flwrs", "flwr", "i", "nm", "usr"
	    };
    std::string compressed_xml = minified_xml;

    // Replace each tag in the string with the compressed version.
    for (size_t i = 0; i < original_tags.size(); ++i) {
            size_t pos = 0;
            // Replace opening tags
            while ((pos = compressed_xml.find("<" + original_tags[i] + ">", pos)) != std::string::npos) {
                compressed_xml.replace(pos, original_tags[i].length() + 2, "<" + compressed_tags[i] + ">");
                pos += compressed_tags[i].length() + 2; // Move past the replaced tag
            }

            pos = 0;
            // Replace closing tags
            while ((pos = compressed_xml.find("</" + original_tags[i] + ">", pos)) != std::string::npos) {
                compressed_xml.replace(pos, original_tags[i].length() + 3, "</" + compressed_tags[i] + ">");
                pos += compressed_tags[i].length() + 3;
            }
        }

        return compressed_xml;
    }







int compress(int argc, char* argv[]) {
    try {
        std::string input_file;
        std::string output_file;

        // Define command-line options
        po::options_description desc("Compress options");
        desc.add_options()
            ("help", "Show help message")
            ("input,i", po::value<std::string>(&input_file)->required(), "Input XML file")
            ("output,o", po::value<std::string>(&output_file)->required(), "Output compressed file");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        po::notify(vm);

        // Open the input XML file
        std::ifstream input(input_file);
        if (!input.is_open()) {
            std::cerr << "Error: Could not open input file\n";
            return EXIT_FAILURE;
        }

        // Read the XML content
        std::string xml((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        input.close();

        // Parse the XML string into a tree
        auto [root, is_valid] = parse_xml(xml);
        if (!is_valid) {
            std::cerr << "Error: Invalid XML file\n";
            return EXIT_FAILURE;
        }

        // Generate the compressed string
        std::string compressed = compress(root);

        // Open the output file
        std::ofstream output(output_file);
        if (!output.is_open()) {
            std::cerr << "Error: Could not open output file\n";
            return EXIT_FAILURE;
        }

        // Write the compressed string to the output file
        output << compressed;
        output.close();

        std::cout << "Compression successful. Output saved to " << output_file << "\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
