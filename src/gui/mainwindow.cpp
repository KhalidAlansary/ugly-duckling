#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <string>

#include "./ui_mainwindow.h"
#include "mainwindow.hpp"
#include "parser.hpp"
#include "prettifier.hpp"

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
  input_file =
      QFileDialog::getOpenFileName(this, "Open File", "", "XML Files (*.xml)")
          .toStdString();
  ui->label->setText(QString::fromStdString(input_file));
}

void MainWindow::on_outputFile_clicked() {
  output_file = QFileDialog::getSaveFileName(this, "Save File").toStdString();
  ui->label_2->setText(QString::fromStdString(output_file));
}

void MainWindow::on_command_currentTextChanged(const QString& arg1) {
  ui->submit->setEnabled(true);
  command = arg1.toStdString();
}

void MainWindow::on_submit_clicked() {
  std::ifstream input(input_file);
  if (!input.is_open()) {
    std::cerr << "Error: Could not open input file\n";
    return;
  }

  std::ofstream output(output_file);
  if (!output.is_open()) {
    std::cerr << "Error: Could not open output file\n";
    return;
  }

  std::string xml((std::istreambuf_iterator<char>(input)),
                  std::istreambuf_iterator<char>());

  input.close();

  auto [root, _] = parse_xml(xml);

  std::string formatted_output = prettify(root);

  output << formatted_output;

  output.close();
}
