#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QPainter>

int NodePixmapFlag=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton_node, &QPushButton::clicked,this,&MainWindow::pushButtonNodeSlot);
    QObject::connect(ui->pushButton_line, &QPushButton::clicked,this,&MainWindow::pushButtonLineSlot);

    //setCursor(QCursor(Qt::WaitCursor));
    scribbleArea = new ScribbleArea;
    //setCentralWidget(scribbleArea);

    //set layout
    ui->gridLayout->addWidget(scribbleArea,0,1);
    ui->gridLayout->setColumnStretch(1,8);
}

void MainWindow::pushButtonNodeSlot()
{
    //set cursor
    QCursor cursor;
    QPixmap pixmap;
    pixmap.load(":/images/node.png");
    cursor = QCursor(pixmap, -1, -1);
    setCursor(cursor);
    NodePixmapFlag = 1;
}

void MainWindow::pushButtonLineSlot()
{
    //set cursor
    QCursor cursor;
    QPixmap pixmap;
    pixmap.load(":/images/line_16x16.png");
    cursor = QCursor(pixmap, -1, -1);
    setCursor(cursor);
    NodePixmapFlag = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
