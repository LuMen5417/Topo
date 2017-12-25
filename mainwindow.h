#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

 private slots:
    void pushButtonNodeSlot();
    void pushButtonLineSlot();

private:
    Ui::MainWindow *ui;
    ScribbleArea *scribbleArea;
};

#endif // MAINWINDOW_H
