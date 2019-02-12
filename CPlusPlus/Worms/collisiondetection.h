#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <QWidget>
#include <QVector>
#include <QImage>

#include "worm.h"
#include "player.h"
#include "bullet.h"

class CollisionDetection : public QWidget
{
    Q_OBJECT
public:
    CollisionDetection(QWidget *parent = 0);

    void setValues(QVector<QVector<int>> AM , QList<Player*> *p , QImage m);
    void freefallDetection(Worm *w);
    void freefallCollision(Worm *w);
    void MapDestuction(Bullet *b);
    void MapDestuction(int xpoint , int ypoint , int Radius);
    int  movementCollision(Worm *w);
    bool onMap(Worm *w);
    bool bulletWormCollision(Bullet *b);
    bool bulletTerrainCollision(Bullet *b);

private:

    QVector<QVector<int>> ArrMap;
    QList<Player*> *players;
    QImage Map;
    void DamageCalculator(Bullet *b);

signals:
    void MovementCollisionResults(int Num);
    void updateMap(QImage map);
    void WormOutofBounds(Player *p , QList<Worm*> *ws , Worm *w);
};

#endif // COLLISIONDETECTION_H
