#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:

  void on_chooseFile_clicked();

  void on_outputFile_clicked();

  void on_command_currentTextChanged(const QString &arg1);

  void on_submit_clicked();

 private:
  Ui::MainWindow* ui;
};
#endif
