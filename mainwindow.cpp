#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QtWidgets>
#include <QPainter>

int NodePixmapFlag=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton_node, &QPushButton::clicked,this,&MainWindow::pushButtonNodeSlot);
    QObject::connect(ui->pushButton_line, &QPushButton::clicked,this,&MainWindow::pushButtonLineSlot);
    QObject::connect(ui->pushButton_delete, &QPushButton::clicked,this,&MainWindow::pushButtonDeleteSlot);
    QObject::connect(ui->pushButton_configure, &QPushButton::clicked,this,&MainWindow::pushButtonConfigureSlot);
    QObject::connect(ui->pushButton_arrow, &QPushButton::clicked,this,&MainWindow::pushButtonArrowSlot);

    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    //setCursor(QCursor(Qt::WaitCursor));
    scribbleArea = new ScribbleArea;
    //setCentralWidget(scribbleArea);

    //set layout
    ui->gridLayout->addWidget(scribbleArea,0,1);
    ui->gridLayout->setColumnStretch(1,12);

    connect(ui->actionClear_Screen, SIGNAL(triggered()),scribbleArea, SLOT(clearImage()));
}

void MainWindow::closeEvent(QCloseEvent *event)
//! [1] //! [2]
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::saveAs()
{
    saveFile("png");
}

void MainWindow::about()
//! [11] //! [12]
{
    QMessageBox::about(this, tr("About Topo"),
            tr("<p>Mesh Network Router Configure Software.</p>"));
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

void MainWindow::pushButtonDeleteSlot()
{
    //set cursor
    QCursor cursor;
    QPixmap pixmap;

    pixmap.load(":/images/delete_16x16.png");
    cursor = QCursor(pixmap, -1, -1);
    setCursor(cursor);
    NodePixmapFlag = 0;
}

void MainWindow::pushButtonConfigureSlot()
{
    //set cursor
    setCursor(QCursor(Qt::WaitCursor));
    NodePixmapFlag = 0;
}

void MainWindow::pushButtonArrowSlot()
{
    //set cursor
    setCursor(QCursor(Qt::ArrowCursor));
    NodePixmapFlag = 0;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
//! [19] //! [20]
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}

bool MainWindow::maybeSave()
//! [17] //! [18]
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}
