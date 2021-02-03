#include <QMouseEvent>
#include <QDebug>
#include "clickableview.h"
#include <QPointF>

ClickableView::ClickableView(QWidget *parent):QGraphicsView(parent) {}

void ClickableView::mousePressEvent(QMouseEvent *event) {
    QPointF p = mapToScene(event->pos());
    if (p.x() >= 0 && p.y() >= 0 && p.x() <= scene()->width() && p.y() <= scene()->height())
        emit mouseClicked(p.y() / 16, p.x() / 16);
}
