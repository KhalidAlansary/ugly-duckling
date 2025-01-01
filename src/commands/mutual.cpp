#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "common.hpp"
#include "parser.hpp"
#include "util.hpp"

namespace po = boost::program_options;

int mutual(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string user_ids;

    po::options_description desc("Mutual options");
    desc.add_options()("help", "Show this help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("ids", po::value<std::string>(&user_ids)->required(),
                          "List of user IDs to get common followers for");

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

    std::vector<std::string> ids;
    boost::split(ids, user_ids, boost::is_any_of(","));

    std::unordered_set<std::string> common_followers = common(users, ids);

    if (common_followers.empty()) {
      std::cout << "No common followers found\n";
    } else {
      std::cout << "Common followers ids:\n";
      for (const std::string& follower : common_followers) {
        std::cout << follower << '\n';
      }
    }

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
