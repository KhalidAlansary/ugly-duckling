#include <boost/program_options.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "decompressor.hpp"
#include "minifier.hpp"

namespace po = boost::program_options;

std::string decompress_xml(const std::string& xml) {
  // Step 1: Parse the XML string into a tree structure.
  auto [root, valid] = parse_xml(xml);

  // Check if the parsing was successful
  if (!valid) {
    return "";
  }
  // Step 2: Compress the tree in place.
  decompress_tree_in_place(root);

  // Step 3: Minify the compressed tree back into a string.
  return minify(root);
}

void decompress_tree_in_place(ElementNode& root) {
  // Define the mapping of compressed to original tags
  static const std::unordered_map<std::string, std::string> tag_map = {
      {"ps", "posts"},      {"pt", "post"},   {"bd", "body"},
      {"tp", "topics"},     {"tpc", "topic"}, {"flwrs", "followers"},
      {"flwr", "follower"}, {"i", "id"},      {"nm", "name"},
      {"usr", "user"}};

  // Helper function for recursive decompression
  std::function<void(ElementNode&)> helper = [&](ElementNode& node) {
    // Decompress the current node's tag name
    auto it = tag_map.find(node.tag_name);
    if (it != tag_map.end()) {
      node.tag_name = it->second;
    }

    // Recursively decompress all children
    for (Node* child : node.children) {
      if (ElementNode* element_child = dynamic_cast<ElementNode*>(child)) {
        helper(*element_child);
      } else if (LeafNode* leaf_child = dynamic_cast<LeafNode*>(child)) {
        // Decompress the tag name of the LeafNode
        auto leaf_it = tag_map.find(leaf_child->tag_name);
        if (leaf_it != tag_map.end()) {
          leaf_child->tag_name = leaf_it->second;
        }
      }
    }
  };

  // Start the decompression process from the root
  helper(root);
}

int decompress(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string output_file;

    // Define command-line options
    po::options_description desc("Decompress options");
    desc.add_options()("help", "Show help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input compressed file")(
        "output,o", po::value<std::string>(&output_file)->required(),
        "Output decompressed XML file");

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
    std::string compressed((std::istreambuf_iterator<char>(input)),
                           std::istreambuf_iterator<char>());
    input.close();

    // Generate the decompressed string
    std::string decompressed = decompress_xml(compressed);

    // Open the output file
    std::ofstream output(output_file);
    if (!output.is_open()) {
      std::cerr << "Error: Could not open output file\n";
      return EXIT_FAILURE;
    }

    // Write the decompressed string to the output file
    output << decompressed;
    output.close();

    std::cout << "Decompression successful. Output saved to " << output_file
              << "\n";
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
