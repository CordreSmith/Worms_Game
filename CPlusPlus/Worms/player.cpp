#include "player.h"

Player::Player(QWidget *parent)
{
    setParent(parent);
    theParent = parent;
    Active = false;
    Alive = true;
    worms = new QList<Worm*>();
}

void Player::setName(QString N)
{
    Name = N;
}

void Player::setNumber(int Num)
{
    PlayerNumber = Num;
}

void Player::setActive()
{
    Active = true;
}

void Player::setNotActive()
{
    Active = false;
}

QString Player::getName()
{
    return(Name);
}

bool Player::getStatus()
{
    return(Active);
}

QString Player::returnName()
{
    return(Name);
}

QList<Worm*>* Player::returnWormList()
{
    return (worms);
}

void Player::spawnWorms(int Amount)
{
    for (int i = 1 ; i <= Amount ; i++)
    {
        Worm *worm = new Worm(theParent);
        switch(PlayerNumber)
        {
        case 1:
            worm->HealthDisplay->setStyleSheet("color : orange");
            worm->NameDisplay->setStyleSheet("color : red");
            break;
        case 2:
            worm->HealthDisplay->setStyleSheet("color : orange");
            worm->NameDisplay->setStyleSheet("color : blue");
            break;
        case 3:
            worm->HealthDisplay->setStyleSheet("color : orange");
            worm->NameDisplay->setStyleSheet("color : green");
            break;
        case 4:
            worm->HealthDisplay->setStyleSheet("color : orange");
            worm->NameDisplay->setStyleSheet("color : yellow");
            break;
        }

        if ( i == 1 && PlayerNumber == 1 )
            worm->Active = true;
        worms->append(worm);
    }
}
