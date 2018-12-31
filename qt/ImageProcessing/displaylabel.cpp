#include "displaylabel.h"
#include <QPainter>

displayLabel::displayLabel(QWidget* parent) : QLabel (parent), mouse_enabled(false)
{

}

void displayLabel::mousePressEvent(QMouseEvent* e)
{
    QLabel::mousePressEvent(e);

    if (!mouse_enabled) return;
    points.push_back(e->pos());
    if (points.size() == 8) {
        update();
        mouse_enabled = false;
        emit sendPoints(points);
        points.clear();
    }
    update();
}

void displayLabel::paintEvent(QPaintEvent *)
{
   if (pixmap())
       setAttribute(Qt::WA_OpaquePaintEvent); /// don't erase previous content
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);
   painter.setRenderHint(QPainter::Qt4CompatiblePainting, true);

   if (pixmap() && points.empty())
       painter.drawPixmap(0,0,pixmap()->width(),pixmap()->height(), *pixmap());
 //  else if (!pixmap())
 //      painter.drawPixmap(0,0,width(),height(), QPixmap(":/background.jpg"));


   if (!points.empty()) {
       const QPoint& p = points.back();
       QBrush cetkica=QBrush(Qt::red);
       painter.setBrush(cetkica);
       QPen pen(Qt::black,1.5);
       painter.setPen(pen);

       //draw a vertex on mouse press
       painter.drawEllipse(p.x()-5,p.y()-5,10,10);
       pen.setColor(Qt::green);
       painter.setPen(pen);

       painter.drawText(QPoint(p.x()-10,p.y()-10), QString::number(points.size()));
   }
}

