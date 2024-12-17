#include <argp.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"

static const char* doc = "Verify the XML file and optionally fix it";

static struct argp_option options[] = {
    {"input", 'i', "FILE", 0, "Input XML file", 0},
    {"output", 'o', "FILE", 0, "Output file", 0},
    {"fix", 'f', 0, 0, "Fix inconsistencies in the XML file", 0},
    {0, 0, 0, 0, 0, 0},
};

struct arguments {
  std::string input = "";
  std::string output = "";
  bool fix = false;
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
    case ARGP_KEY_ARG:
      std::cerr << "Error: Invalid argument\n";
      argp_usage(state);
      break;
    case ARGP_KEY_END:
      if (arguments->input.empty()) {
        std::cerr << "Error: Missing input file\n";
        argp_usage(state);
      }
      if (arguments->output.empty() == arguments->fix) {
        std::cerr << "fix and output must be used together\n";
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};

int verify(int argc, char* argv[]) {
  struct arguments argz;
  argp_parse(&argp, argc, argv, 0, 0, &argz);

  std::ifstream input(argz.input);
  if (!input.is_open()) {
    std::cerr << "Error: Could not open input file\n";
    return EXIT_FAILURE;
  }
  std::ofstream output;
  if (!argz.output.empty()) {
    output.open(argz.output);
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

  if (argz.fix) {
    // TODO: Put the formatted representation of root into output
    std::cerr << "Not yet implemented\n";
    return EXIT_FAILURE;
  }

  if (output.is_open()) {
    output.close();
  }

  return EXIT_SUCCESS;
}
