#ifndef MOUSEEVENTHANDLER_H
#define MOUSEEVENTHANDLER_H

#include <QWidget>
#include <QMouseEvent>
#include <QCursor>
#include <QProgressbar>
#include <QTimer>

class MouseEventHandler : public QWidget
{
    Q_OBJECT
public:
    MouseEventHandler(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int cursorx , cursory ;
    double projectilexSpeed , projectileySpeed , Strengh ;
    bool SpawnProgressBar;
    QProgressBar *StrenghMeter;
    QTimer *StrenghTimer;

signals:
    void SpawnBullet(int xPos , int yPos , double Strengh);

public slots:
    void activate(bool PBstatus);
    void hideProgressBar();

private slots:
    void updateStrengh();

};

#endif // MOUSEEVENTHANDLER_H
