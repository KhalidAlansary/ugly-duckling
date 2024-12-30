#include <QFileDialog>

#include "./ui_mainwindow.h"
#include "mainwindow.hpp"

extern const std::unordered_map<std::string, int (*)(int, char**)> commands;

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

void MainWindow::on_chooseFile_clicked() {}

void MainWindow::on_outputFile_clicked() {}
