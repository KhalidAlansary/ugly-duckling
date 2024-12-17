#include <argp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

#include "commands.hpp"

static const char* doc =
    "XML Parser -- A simple yet powerful XML utility.\n\n"
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
    "  mutual            Writes a list of the mutual users between users with "
    "ids\n";

static const char* args_doc = "<command> [options]";

static std::string command = "";

// Maps commands to functions
static const std::unordered_map<std::string, int (*)(int, char**)> commands = {
    {"verify", verify},
    // TODO: Uncomment the following lines after implementing the functions
    // {"format", format},
    // {"json", json},
    // {"mini", mini},
    // {"compress", compress},
    // {"decompress", decompress},
    // {"draw", draw},
    // {"most_active", most_active},
    // {"most_influential", most_influential},
    // {"mutual", mutual},
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
  switch (key) {
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
        argp_usage(state);
      }
      command = arg;
      break;
    case ARGP_KEY_END:
      if (command.empty()) {
        std::cerr << "Error: Missing command\n";
        argp_usage(state);
      }
      if (!commands.contains(command)) {
        std::cerr << "Error: Invalid command\n";
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct argp argp = {0, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char* argv[]) {
  int arg_index;
  argp_parse(&argp, std::min(argc, 2), argv, ARGP_IN_ORDER, &arg_index, 0);

  // Call the command
  return commands.at(command)(argc - 1, argv + 1);
}
