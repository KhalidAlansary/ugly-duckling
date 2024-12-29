#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"
#include "suggester.hpp"
#include "util.hpp"

namespace po = boost::program_options;

int suggest(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string user_id;

    po::options_description desc("Suggest options");
    desc.add_options()("help", "Show this help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("id", po::value<std::string>(&user_id)->required(),
                          "User ID to get suggestions for");

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

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, _] = parse_xml(xml);
    std::vector<User> users =
        parse_users(*dynamic_cast<ElementNode*>(root.children.front()));

    std::cout << suggest(users, user_id) << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}