#include "maplabel.h"

MapLabel::MapLabel(QWidget *parent) : QLabel(parent)
{

}

void MapLabel::setFileName(QString FN)
{
    FileName = FN;
}

void MapLabel::setName(QString N)
{
    Name = N;
}

void MapLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(FileName,Name);
}
