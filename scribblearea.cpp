/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include <QtDebug>
#include <QPointF>

#include "scribblearea.h"

extern int NodePixmapFlag;

unsigned int IPv4addrPool;
unsigned int IPv4addrMask;
int nodePool;
int linkPool;

void list_add(nodeList &listcopy, const Rnode tempnode)
{
    listcopy.push_front(tempnode);
}

unsigned int get_ipv4addr(int number)
{
    return IPv4addrPool+number;
}

unsigned int get_nodenumber()
{
    nodePool +=1;
    return nodePool;
}

unsigned int get_ipv4mask()
{
    return IPv4addrMask;
}

Rnode getRouterNode(nodeList &listcopy, QPoint point)
{
    Rnode tempnode;
    int x,y,temp_x,temp_y;

    tempnode.number = 0;

    if(listcopy.isEmpty())
        return tempnode;

    QList<Rnode>::iterator i;

    for(i = listcopy.begin(); i != listcopy.end(); ++i){
         x = (*i).point.x();
         y = (*i).point.y();

         temp_x = point.x();
         temp_y = point.y();

         if((x-16 < temp_x) && (temp_x < x+16))
            if((y-16 < temp_y) && (temp_y < y + 16)){
                tempnode = (*i);
                return tempnode;
         }
    }

    return tempnode;
}

//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 2;
    myPenColor = Qt::green;

    linkPool = 0;
    nodePool = 0;
    IPv4addrPool = 0x0a000064;
    IPv4addrMask = 0xffffff00;
}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
//! [4]

//! [5]
void ScribbleArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    myPenColor = newColor;
}
//! [6]

//! [7]
void ScribbleArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    myPenWidth = newWidth;
}
//! [8]

//! [9]
void ScribbleArea::clearImage()
//! [9] //! [10]
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
//! [10]

//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    int num = 0;
    Rnode node;

    if (event->button() == Qt::LeftButton) {

        if (NodePixmapFlag != 1){
            startPoint = event->pos();

            node = getRouterNode(listRouter,startPoint);
            num = node.number;
            qDebug()<<"The number:"<<num;
            if(num != 0){
                scribbling = true;
                curLink.startNumer = node.number;
                curLink.startPoint = node.point;
            }else
                scribbling = false;
        }else{
            //qDebug()<<"The NodePixmapFlag:"<<NodePixmapFlag;
            startPoint = event->pos();
            Rnode tempnode;
            tempnode.number = get_nodenumber();
            tempnode.ipv4addr = get_ipv4addr(tempnode.number);
            tempnode.ipv4mask = get_ipv4mask();
            tempnode.point = startPoint;

            list_add(listRouter, tempnode);
            drawPixmapTo(startPoint, tempnode);
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineMove(event->pos());
        //drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint endPoint;
    Rnode node;
    int num;

    if (event->button() == Qt::LeftButton && scribbling) {
        endPoint = event->pos();
        node = getRouterNode(listRouter,endPoint);
        num = node.number;
        if(num != 0){
            drawLineTo(endPoint);
            scribbling = false;
        }
    }
}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//! [16]

//! [17]
void ScribbleArea::drawLineTo(const QPoint &endPoint)
//! [17] //! [18]
{

    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawLine(startPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(startPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    startPoint = endPoint;
}
//! [18]

void ScribbleArea::drawLineMove(const QPoint &endPoint)
//! [17] //! [18]
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    //painter.drawLine(startPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(startPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    //update();
    lastPoint=endPoint;
}

void ScribbleArea::drawPixmapTo(const QPoint &endPoint, Rnode &tempnode)
{
    QPainter painter(&image);
    QPixmap pix;
    pix.load(":/images/node_64x64.png");
    int x = endPoint.x();
    int y = endPoint.y();
    painter.drawPixmap(x-32,y-32,pix);

    unsigned int ipv4addr = tempnode.ipv4addr;

    char stripv4addr[16]={0};
    unsigned char *charipv4addr =  (unsigned char*)(&ipv4addr);
    qDebug()<<"ipv4addr:"<<hex<<charipv4addr[0]<<endl;
    qDebug()<<"ipv4addr:"<<hex<<charipv4addr[1]<<endl;
    qDebug()<<"ipv4addr:"<<hex<<charipv4addr[2]<<endl;
    qDebug()<<"ipv4addr:"<<hex<<charipv4addr[3]<<endl;

    sprintf(stripv4addr,"%d.%d.%d.%d/24",charipv4addr[3], charipv4addr[2], charipv4addr[1], charipv4addr[0]);
    qDebug()<<stripv4addr<<endl;
    painter.drawText(x-40,y+36,QString(stripv4addr));
    update();
}


//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
//! [20]

//! [21]
void ScribbleArea::print()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
//! [22]
