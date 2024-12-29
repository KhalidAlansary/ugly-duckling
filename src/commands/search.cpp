#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"
#include "search.hpp"
#include "util.hpp"

namespace po = boost::program_options;

int search(int argc, char* argv[]) {
  try {
    std::string input_file;
    std::string word;
    std::string topic;

    po::options_description desc("Format options");
    desc.add_options()("help", "Show this help message")(
        "input,i", po::value<std::string>(&input_file)->required(),
        "Input XML file")("word,w", po::value<std::string>(&word),
                          "word to search for")(
        "topic,t", po::value<std::string>(&topic), "topic to search for");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return EXIT_SUCCESS;
    }

    po::notify(vm);

    if (word.empty() == topic.empty()) {
      std::cerr << "word or topic only not both\n";
      std::cout << desc << "\n";
      return EXIT_FAILURE;
    }

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

    std::vector<Post> posts;
    if (!word.empty()) {
      posts = word_search(users, word);
    } else {
      posts = topic_search(users, topic);
    }

    for (const Post& post : posts) {
      std::cout << post.body << std::endl;
    }

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}