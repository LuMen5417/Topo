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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void about();
    void saveAs();
    void pushButtonNodeSlot();
    void pushButtonLineSlot();
    void pushButtonConfigureSlot();
    void pushButtonDeleteSlot();

private:
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    Ui::MainWindow *ui;
    ScribbleArea *scribbleArea;
};

#endif // MAINWINDOW_H
