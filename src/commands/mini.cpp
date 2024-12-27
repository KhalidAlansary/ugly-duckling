#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "minifier.hpp"
#include "parser.hpp"

namespace po = boost::program_options;

int mini(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string output_file;

    po::options_description desc("Format options");
    desc.add_options()("help", "Show this help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("output,o",
                          po::value<std::string>(&output_file)->required(),
                          "Output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return EXIT_SUCCESS;
    }

    po::notify(vm);

    std::ifstream input(input_file);
    if (!input.is_open()) {
      std::cerr << "Error: Could not open input file\n";
      return EXIT_FAILURE;
    }

    std::ofstream output(output_file);
    if (!output.is_open()) {
      std::cerr << "Error: Could not open output file\n";
      return EXIT_FAILURE;
    }

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, _] = parse_xml(xml);

    std::string minified_xml = minify(root);

    output << minified_xml;

    output.close();
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
