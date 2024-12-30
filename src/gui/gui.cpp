#include <QApplication>

#include "mainwindow.hpp"

int GUI(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
