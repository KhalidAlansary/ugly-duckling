#include <QFileDialog>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "./ui_mainwindow.h"
#include "commands.hpp"
#include "common.hpp"
#include "compressor.hpp"
#include "converter.hpp"
#include "decompressor.hpp"
#include "mainwindow.hpp"
#include "minifier.hpp"
#include "parser.hpp"
#include "prettifier.hpp"
#include "search.hpp"
#include "suggester.hpp"

extern const std::unordered_map<std::string, int (*)(int, char**)> commands;
std::string command;
std::string input_file;
std::string output_file;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  for (const auto& [command, _] : commands) {
    ui->command->addItem(QString::fromStdString(command));
  }
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_chooseFile_clicked() {
  input_file = QFileDialog::getOpenFileName(this, "Open XML File", "",
                                            "XML Files (*.xml)")
                   .toStdString();
  ui->label->setText(QString::fromStdString(input_file));
  ui->submit->setEnabled(!input_file.empty() && !command.empty());
}

void MainWindow::on_outputFile_clicked() {
  output_file = QFileDialog::getSaveFileName(this, "Save Output File", "",
                                             "Text Files (*.txt)")
                    .toStdString();
  ui->label_2->setText(QString::fromStdString(output_file));
  ui->submit->setEnabled(!input_file.empty() && !command.empty());
}

void MainWindow::on_command_currentTextChanged(const QString& arg1) {
  command = arg1.toStdString();
  ui->submit->setEnabled(!input_file.empty() && !command.empty());
}

void MainWindow::on_submit_clicked() {
  executeCommand();
}

void MainWindow::executeCommand() {
  std::ifstream input(input_file);
  if (!input.is_open()) {
    std::cerr << "Error: Could not open input file\n";
    return;
  }

  std::string xml((std::istreambuf_iterator<char>(input)),
                  std::istreambuf_iterator<char>());

  input.close();

  auto [root, _] = parse_xml(xml);

  std::string result;

  // Get user input from QLineEdit
  std::string user_input = ui->userInput->text().toStdString();

  // Prepare arguments based on the command
  std::vector<std::string> args = {command};
  if (command == "most_active" || command == "most_influencer") {
    args.push_back("-i");
    args.push_back(input_file);
    if (!user_input.empty()) {
      if (command == "mutual") {
        args.push_back("-ids");
      } else if (command == "suggest") {
        args.push_back("-id");
      }
      args.push_back(user_input);
    }
  } else if (command == "search") {
    if (!user_input.empty()) {
      if (user_input.find(" ") != std::string::npos) {
        args.push_back("-t");
      } else {
        args.push_back("-w");
      }
      args.push_back(user_input);
    }
    args.push_back("-i");
    args.push_back(input_file);
  } else {
    args.push_back(input_file);
  }

  std::vector<char*> argv;
  for (auto& arg : args) {
    argv.push_back(&arg[0]);
  }
  argv.push_back(nullptr);

  int argc = argv.size() - 1;

  if (command == "format") {
    result = prettify(root);
  } else if (command == "compress") {
    result = compress_xml(xml);
  } else if (command == "decompress") {
    result = decompress_xml(xml);
  } else if (command == "mini") {
    result = minify(root);
  } else if (command == "json") {
    result = convert(root);
  } else if (command == "draw") {
    draw(argc, argv.data());
    result = "Draw command executed.";
  } else if (command == "most_active") {
    result = most_active(argc, argv.data());
  } else if (command == "most_influencer") {
    result = most_influencer(argc, argv.data());
  } else if (command == "mutual") {
    std::ifstream input(input_file);
    if (!input.is_open()) {
      std::cerr << "Error: Could not open input file\n";
      return;
    }

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, _] = parse_xml(xml);
    std::vector<User> users =
        parse_users(*dynamic_cast<ElementNode*>(root.children.front()));

    std::vector<std::string> ids;
    boost::split(ids, ui->userInput->text().toStdString(),
                 boost::is_any_of(","));

    std::unordered_set<std::string> common_followers = common(users, ids);

    if (common_followers.empty()) {
      result = "No common followers found";
    } else {
      result = "Common followers ids:\n";
      for (const std::string& follower : common_followers) {
        result += follower + '\n';
      }
    }
  } else if (command == "suggest") {
    std::ifstream input(input_file);
    if (!input.is_open()) {
      std::cerr << "Error: Could not open input file\n";
      return;
    }

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, _] = parse_xml(xml);
    std::vector<User> users =
        parse_users(*dynamic_cast<ElementNode*>(root.children.front()));

    std::unordered_set<std::string> suggestions =
        suggest(users, ui->userInput->text().toStdString());

    if (suggestions.empty()) {
      result = "No suggestions available for this user";
    } else {
      result = "Suggestions:\n";
      for (const std::string& suggestion : suggestions) {
        result += suggestion + '\n';
      }
    }
  } else if (command == "search") {
    std::ifstream input(input_file);
    if (!input.is_open()) {
      std::cerr << "Error: Could not open input file\n";
      return;
    }

    std::string xml((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());

    input.close();

    auto [root, _] = parse_xml(xml);
    std::vector<User> users =
        parse_users(*dynamic_cast<ElementNode*>(root.children.front()));
    std::vector<Post> posts =
        word_search(users, ui->userInput->text().toStdString());

    for (const Post& post : posts) {
      result += post.body + "\n";
    }
  } else if (command == "verify") {
    int verify_result = verify(argc, argv.data());
    result = (verify_result == 0) ? "valid" : "invalid";
  } else {
    std::cerr << "Unknown command\n";
    return;
  }

  // Display the result in the QTextEdit widget
  ui->outputTextEdit->setPlainText(QString::fromStdString(result));

  // Only write to the output file if it's not one of the specified commands
  if (command != "search" && command != "most_active" &&
      command != "most_influencer" && command != "suggest" &&
      command != "verify") {
    std::ofstream output(output_file);
    if (!output.is_open()) {
      std::cerr << "Error: Could not open output file\n";
      return;
    }
    output << result;
    output.close();
  }
}