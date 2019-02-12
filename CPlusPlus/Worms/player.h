#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

#include "worm.h"

class Player : public QWidget
{
    Q_OBJECT
public:
    Player(QWidget *parent = 0);

    void setName(QString N);
    void setNumber(int Num);
    void setActive();
    void setNotActive();
    void setAlive(bool A);
    void spawnWorms(int Amount);
    bool getAlive();
    bool getStatus();
    QString getName();
    QString returnName ();
    QList<Worm *> *returnWormList();

private:
    int PlayerNumber;

    QList<Worm*> *worms;
    QWidget *theParent;
    QString Name;
    bool Active , Alive ;

};

#endif // PLAYER_H
