#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <QKeyEvent>
#include <QWidget>

#include "worm.h"

class KeyHandler : public QWidget
{
    Q_OBJECT
public:
    KeyHandler(QWidget *parent = 0);

private:
    Worm *ActiveWorm;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void ActiveWormUpdated(Worm *w);

signals:
    void EndTurn();
    void GunListShow();
};

#endif // KEYHANDLER_H
