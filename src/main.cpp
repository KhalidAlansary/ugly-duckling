#include <argp.h>
#include <cstdlib>
#include <string>

static const char* doc = "XML Parser -- A simple yet powerful XML utility.";

static const char* args_doc = "<command> [options]";

struct argp_option options[] = {
    {"input", 'i', "FILE", 0, "Input XML file", 0},
    {"output", 'o', "FILE", 0, "Output file", 0},
    {"fix", 'f', 0, 0, "Fix inconsistencies the XML file", 0},
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
      // TODO: Check for required arguments.
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

  // TODO: Implement the logic.

  return EXIT_SUCCESS;
}
