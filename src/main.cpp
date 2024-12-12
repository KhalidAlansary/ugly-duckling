#include <argp.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "compresser.hpp"
#include "formatter.hpp"
#include "parser.hpp"

// NOTE: Probably would be better to divide the program into subprograms. Each
// with its own arguments parsing.
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

struct argp_option options[] = {
    {"input", 'i', "FILE", 0, "Input XML file", 0},
    {"output", 'o', "FILE", 0, "Output file", 0},
    {"fix", 'f', 0, 0, "Fix inconsistencies in the XML file", 0},
    {"ids", 's', "ID_LIST", 0, "IDs of the users", 0},
    {"id", 'd', "ID", 0, "ID of the user", 0},
    {"word", 'w', "WORD", 0, "Word to search", 0},
    {"topic", 't', "TOPIC", 0, "Topic to search", 0},
    {0, 0, 0, 0, 0, 0},
};

struct arguments {
  std::string command = "";
  std::string input = "";
  std::string output = "";
  bool fix = false;
  std::string ids = "";
  int id = -1;
  std::string word = "";
  std::string topic = "";
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
  struct arguments* arguments = static_cast<struct arguments*>(state->input);

  switch (key) {
    case 'i':
      arguments->input = arg;
      break;
    case 'o':
      arguments->output = arg;
      break;
    case 'f':
      arguments->fix = true;
      break;
    case 's':
      arguments->ids = arg;
      break;
    case 'd':
      // WARNING: No error checking.
      arguments->id = std::stoi(arg);
      break;
    case 'w':
      arguments->word = arg;
      break;
    case 't':
      arguments->topic = arg;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
        argp_usage(state);
      }
      arguments->command = arg;
      break;
    case ARGP_KEY_END:
      if (arguments->command.empty()) {
        argp_usage(state);
      }
      // NOTE: Better to check for required arguments here but whatever.
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char* argv[]) {
  struct arguments argz;
  argp_parse(&argp, argc, argv, 0, 0, &argz);

  if (argz.command == "verify") {
    if (argz.input.empty()) {
      std::cerr << "Error: Missing input file\n";
      return EXIT_FAILURE;
    }
    if (argz.fix && argz.output.empty()) {
      std::cerr << "Error: Missing output file\n";
      return EXIT_FAILURE;
    }
    std::ifstream input_file(argz.input);
    if (!input_file) {
      std::cerr << "Error: Could not open input file\n";
      return EXIT_FAILURE;
    }
    std::string input_xml((std::istreambuf_iterator<char>(input_file)),
                          std::istreambuf_iterator<char>());
    input_file.close();

    std::tuple<XMLNode, bool> result = parse_xml(input_xml);
    std::cout << (std::get<1>(result) ? "valid" : "invalid") << std::endl;
    if (argz.fix) {
      std::ofstream output_file(argz.output);
      if (!output_file) {
        std::cerr << "Error: Could not open output file\n";
        return EXIT_FAILURE;
      }
      output_file << prettify(std::get<0>(result));
      output_file.close();
    }
  } else if (argz.command == "compress") {
    if (argz.input.empty()) {
      std::cerr << "Error: Missing input file\n";
      return EXIT_FAILURE;
    }
    if (argz.output.empty()) {
      std::cerr << "Error: Missing output file\n";
      return EXIT_FAILURE;
    }
    std::ifstream input_file(argz.input);
    if (!input_file) {
      std::cerr << "Error: Could not open input file\n";
      return EXIT_FAILURE;
    }
    std::string input_xml((std::istreambuf_iterator<char>(input_file)),
                          std::istreambuf_iterator<char>());
    input_file.close();

    std::tuple<XMLNode, bool> result = parse_xml(input_xml);
    if (!std::get<1>(result)) {
      std::cerr << "Error: Invalid XML file\n";
      return EXIT_FAILURE;
    }

    std::string compressed = compress(std::get<0>(result));
    std::ofstream output_file(argz.output);
    if (!output_file) {
      std::cerr << "Error: Could not open output file\n";
      return EXIT_FAILURE;
    }
    output_file << compressed;
    output_file.close();
  }
  // TODO: Implement the rest of the commands.

  return EXIT_SUCCESS;
}
