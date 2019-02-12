#include "mouseeventhandler.h"

MouseEventHandler::MouseEventHandler(QWidget *parent)
{
    setParent(parent);
    resize(parent->width(),parent->height());
    show();

    SpawnProgressBar = true;
    this->setEnabled(false);

    // Progress Bar setup //

    StrenghMeter = new QProgressBar (parent);
    StrenghMeter->setFixedWidth(400);
    StrenghMeter->setFixedHeight(30);
    StrenghMeter->setValue(0);
    StrenghMeter->move(parent->width()/2-StrenghMeter->width()/2,40);
    StrenghMeter->show();
    StrenghMeter->setVisible(false);

    // Timers Setup //

    StrenghTimer = new QTimer(this);
    StrenghTimer->setInterval(50);
    connect(StrenghTimer,SIGNAL(timeout()),this,SLOT(updateStrengh()));

}

void MouseEventHandler::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QPoint cursorPosition = mapFromGlobal(QCursor::pos());
    cursorx = cursorPosition.x();
    cursory = cursorPosition.y();

    if (SpawnProgressBar == true)
    {
        StrenghMeter->setVisible(true);
        StrenghMeter->setValue(0);
        StrenghTimer->start();
        Strengh = 5;
    }
}

void MouseEventHandler::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (SpawnProgressBar == true)
    {
        StrenghMeter->setVisible(false);
        StrenghTimer->stop();
    }

    emit SpawnBullet(cursorx,cursory,Strengh);
    this->lower();
    this->setEnabled(false);
}

void MouseEventHandler::activate(bool PBstatus)
{
    this->raise();
    this->setEnabled(true);
    SpawnProgressBar = PBstatus;
}

void MouseEventHandler::hideProgressBar()
{
    StrenghMeter->setVisible(false);
    StrenghMeter->setValue(0);
}

void MouseEventHandler::updateStrengh()
{
    Strengh += 0.5;
    StrenghMeter->setValue(StrenghMeter->value()+4);
}
