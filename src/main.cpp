#include <algorithm>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#include "commands.hpp"

namespace po = boost::program_options;

extern const std::unordered_map<std::string, int (*)(int, char**)> commands = {
    {"verify", verify},
    {"format", format},
    {"json", json},
    {"mini", mini},
    {"compress", compress},
    {"decompress", decompress},
    {"draw", draw},
    {"suggest", suggest},
    {"search", search},
    {"most_active", most_active},
    {"most_influencer", most_influencer},
    // TODO: Uncomment the following lines after implementing the functions
    // {"mutual", mutual},
};

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return GUI(argc, argv);
  }
  try {
    po::options_description desc(
        "XML Parser -- A simple yet powerful XML utility");
    desc.add_options()("help", "Show this help message")(
        "command", po::value<std::string>(),
        "Commands:\n"
        "  verify            Verify the XML file\n"
        "  format            Format the XML file\n"
        "  json              Convert the XML file to JSON\n"
        "  mini              Minify the XML file\n"
        "  compress          Compress the XML file\n"
        "  decompress        Decompress the XML file\n"
        "  draw              Draw the XML file\n"
        "  most_active       Write the most active username and id\n"
        "  most_influential  Write the most influencer username and id\n"
        "  mutual            Writes a list of the mutual users between users "
        "with ids\n"
        "  suggest           Writes a list of suggested users for user with a "
        "given id \n"
        "  search            Writes a list of posts where the given word or "
        "topic was mentioned ");

    po::positional_options_description p;
    p.add("command", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(std::min(argc, 2), argv)
                  .options(desc)
                  .positional(p)
                  .run(),
              vm);
    po::notify(vm);

    if (vm.count("help") || !vm.count("command")) {
      std::cout << desc << "\n";
      return EXIT_SUCCESS;
    }

    std::string command = vm["command"].as<std::string>();
    if (!commands.contains(command)) {
      std::cerr << "Error: Invalid command\n";
      std::cout << desc << "\n";
      return EXIT_FAILURE;
    }

    return commands.at(command)(argc - 1, argv + 1);
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}
