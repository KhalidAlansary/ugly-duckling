#include <boost/program_options.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "compressor.hpp"
#include "minifier.hpp"

namespace po = boost::program_options;

std::string compress_xml(const std::string& xml) {
  // Step 1: Parse the XML string into a tree structure.
  auto [root, valid] = parse_xml(xml);

  // Check if the parsing was successful
  if (!valid) {
    return "";
  }

  // Step 2: Compress the tree in place.
  compress_tree_in_place(root);

  // Step 3: Minify the compressed tree back into a string.
  return minify(root);
}

void compress_tree_in_place(ElementNode& root) {
  // Define the list of original and compressed tags
  std::vector<std::string> original_tags = {
      "posts",     "post",     "body", "topics", "topic",
      "followers", "follower", "id",   "name",   "user"};

  std::vector<std::string> compressed_tags = {
      "ps", "pt", "bd", "tp", "tpc", "flwrs", "flwr", "i", "nm", "usr"};

  // Helper function for recursive compression
  std::function<void(ElementNode&)> helper = [&](ElementNode& node) {
    // Compress the current node's tag name
    for (size_t i = 0; i < original_tags.size(); ++i) {
      if (node.tag_name == original_tags[i]) {
        // Replace the tag name with its compressed version
        const_cast<std::string&>(node.tag_name) = compressed_tags[i];
        break;
      }
    }

    // Recursively compress all children
    for (Node* child : node.children) {
      // Check if the child is an ElementNode
      if (ElementNode* element_child = dynamic_cast<ElementNode*>(child)) {
        helper(*element_child);
      } else if (LeafNode* leaf_child = dynamic_cast<LeafNode*>(child)) {
        // Compress the tag name of the LeafNode
        for (size_t i = 0; i < original_tags.size(); ++i) {
          if (leaf_child->tag_name == original_tags[i]) {
            const_cast<std::string&>(leaf_child->tag_name) = compressed_tags[i];
            break;
          }
        }
      }
    }
  };

  // Start the compression process from the root
  helper(root);
}

int compress(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string output_file;

    // Define command-line options
    po::options_description desc("Compress options");
    desc.add_options()("help", "Show help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("output,o",
                          po::value<std::string>(&output_file)->required(),
                          "Output compressed file");

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
    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
    input.close();

    // Compress XML
    std::string compressed = compress_xml(xml);
    // Open the output file
    std::ofstream output(output_file);
    if (!output.is_open()) {
      std::cerr << "Error: Could not open output file\n";
      return EXIT_FAILURE;
    }

    // Write the compressed string to the output file
    output << compressed;
    output.close();

    std::cout << "Compression successful. Output saved to " << output_file
              << "\n";
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
