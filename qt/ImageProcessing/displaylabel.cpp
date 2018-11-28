#include "displaylabel.h"
#include <QPainter>

displayLabel::displayLabel(QWidget* parent) : QLabel (parent), mouse_enabled(false)
{

}

void displayLabel::mousePressEvent(QMouseEvent* e)
{
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
   setAttribute(Qt::WA_OpaquePaintEvent); /// don't erase previous content
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);
   painter.setRenderHint(QPainter::Qt4CompatiblePainting, true);

   if (pixmap() && points.empty())
       painter.drawPixmap(0,0,pixmap()->width(),pixmap()->height(), *pixmap());


   if (!points.empty()) {
       const QPoint& p = points.back();
       QBrush cetkica=QBrush(Qt::red);
       painter.setBrush(cetkica);
       QPen olovka(Qt::black,1.5);
       painter.setPen(olovka);

       //draw a vertex on mouse press
       painter.drawEllipse(p.x()-5,p.y()-5,10,10);
       olovka.setColor(Qt::green);
       painter.setPen(olovka);

       painter.drawText(QPoint(p.x()-10,p.y()-10), QString::number(points.size()));
   }
}

