#include <boost/program_options.hpp>
#include <functional>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "formatter.hpp"
#include "parser.hpp"

namespace po = boost::program_options;

std::string prettify(const ElementNode& root) {
    std::function<void(const ElementNode&, int, std::ostringstream&)> helper =
        [&helper](const ElementNode& node, int depth, std::ostringstream& oss) -> void {
        oss << std::string(INDENTATION_SIZE * depth, INDENTATION_CHAR);
        oss << node.get_opening_tag() << '\n';
        for (Node* child : node.children) {
            // LeafNode
            if (LeafNode* leaf = dynamic_cast<LeafNode*>(child)) {
                oss << std::string(INDENTATION_SIZE * (depth + 1), INDENTATION_CHAR)
                    << leaf->get_opening_tag();
                if (leaf->content.size() > MAX_LINE_LENGTH) {
                    oss << '\n'
                        << std::string(INDENTATION_SIZE * (depth + 2), INDENTATION_CHAR)
                        << leaf->content << '\n'
                        << std::string(INDENTATION_SIZE * (depth + 1), INDENTATION_CHAR);
                } else {
                    oss << leaf->content;
                }
                oss << leaf->get_closing_tag();
            } else if (ElementNode* elem = dynamic_cast<ElementNode*>(child)) {
                helper(*elem, depth + 1, oss);
            } else if (CommentNode* comment = dynamic_cast<CommentNode*>(child)) {
                oss << std::string(INDENTATION_SIZE * (depth + 1), INDENTATION_CHAR)
                    << "<!-- " << comment->comment << " -->";
            } else {
                throw std::runtime_error("Unknown node type");
            }
            oss << '\n';
        }
        oss << std::string(INDENTATION_SIZE * depth, INDENTATION_CHAR);
        oss << node.get_closing_tag();
    };

    std::ostringstream oss;
    helper(root, 0, oss);
    return oss.str();
}

int format(int argc, char* argv[]) {
    try {
        std::string input_file;
        std::string output_file;

        po::options_description desc("Format options");
        desc.add_options()
            ("help", "Show this help message")
            ("input,i", po::value<std::string>(&input_file)->required(), "Input XML file")
            ("output,o", po::value<std::string>(&output_file), "Output file");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        po::notify(vm);

        std::ifstream input(input_file);
        if (!input.is_open()) {
            std::cerr << "Error: Could not open input file " << input_file << "\n";
            return EXIT_FAILURE;
        }

        std::ofstream output;
        if (vm.count("output")) {
            output.open(output_file);
            if (!output.is_open()) {
                std::cerr << "Error: Could not open output file " << output_file << "\n";
                return EXIT_FAILURE;
            }
        }

        // Read XML input from file
        std::string xml((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

        // Parse the XML input
        auto [root, is_valid] = parse_xml(xml);

        // Check if the XML is valid
        if (!is_valid) {
            std::cerr << "Invalid XML input\n";
            return EXIT_FAILURE;
        }

        // Format the root node
        std::string formatted_output = prettify(root);

        // Output the formatted result
        if (output.is_open()) {
            output << formatted_output;
            output.close();
        } else {
            std::cout << formatted_output << std::endl;
        }

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}