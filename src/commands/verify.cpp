#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"
#include "prettifier.hpp"

namespace po = boost::program_options;

int verify(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string output_file;
    bool fix = false;

    po::options_description desc("Verify options");
    desc.add_options()("help", "Show this help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("output,o", po::value<std::string>(&output_file),
                          "Output file")("fix,f", po::bool_switch(&fix),
                                         "Fix inconsistencies in the XML file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return EXIT_SUCCESS;
    }

    po::notify(vm);

    if (output_file.empty() == fix) {
      std::cerr << "fix and output must be used together\n";
      std::cout << desc << "\n";
      return EXIT_FAILURE;
    }

    std::ifstream input(input_file);
    if (!input.is_open()) {
      std::cerr << "Error: Could not open input file\n";
      return EXIT_FAILURE;
    }

    std::ofstream output;
    if (!output_file.empty()) {
      output.open(output_file);
      if (!output.is_open()) {
        std::cerr << "Error: Could not open output file\n";
        return EXIT_FAILURE;
      }
    }

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, is_valid] = parse_xml(xml);

    std::cout << (is_valid ? "valid" : "invalid") << '\n';

    if (fix) {
      output << prettify(root);
    }

    if (output.is_open()) {
      output.close();
    }

    return EXIT_SUCCESS;
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
