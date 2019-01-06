#ifndef DISPLAYLABEL_H
#define DISPLAYLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QVector>

class displayLabel : public QLabel
{
	Q_OBJECT

public:
	displayLabel(QWidget* parent = nullptr);
	bool mouse_enabled;
protected:
	void mousePressEvent(QMouseEvent* e);
	void paintEvent(QPaintEvent* e);
private:
	QVector<QPoint> points;
signals:
	void sendPoints(QVector<QPoint>& v);
};

#endif // DISPLAYLABEL_H
