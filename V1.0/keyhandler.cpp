#include "keyhandler.h"

KeyHandler::KeyHandler(QWidget *parent)
{
    setParent(parent);
    show();
    setFocus();
}

void KeyHandler::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch(event->key())
    {
    case Qt::Key_Left:
        ActiveWorm->Left = true;
        ActiveWorm->ApplyTexture(1);
        break;
    case Qt::Key_Right:
        ActiveWorm->Right = true;
        ActiveWorm->ApplyTexture(2);
        break;
    case Qt::Key_Space:
        ActiveWorm->Jump = true;
        break;
    case Qt::Key_End:
        emit EndTurn();
        break;
    case Qt::Key_G:
        emit GunListShow();
        ActiveWorm->Left = false;
        ActiveWorm->Right = false;
        ActiveWorm->Jump = false;
        break;
    default :
        QWidget::keyPressEvent(event);
    }
}

void KeyHandler::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch(event->key())
    {
    case Qt::Key_Left:
        ActiveWorm->Left = false;
        break;
    case Qt::Key_Right:
        ActiveWorm->Right = false;
        break;
    case Qt::Key_Space:
        ActiveWorm->Jump = false;
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void KeyHandler::ActiveWormUpdated(Worm *w)
{
    ActiveWorm = w;
}
