#include "worm.h"

#include <QPixmap>
#include <QColor>
#include <QRgb>

Worm::Worm(QWidget *parent) : QLabel(parent)
{
    Left      = false;
    Right     = false;
    Jump      = false;
    Active    = false;
    onGround  = false;
    Direction = false;
    Health = 100;
    ySpeed = 0;
    xSpeed = 0;

    HealthDisplay = new QLabel (parent);
    NameDisplay   = new QLabel (parent);

    Names.append("Isaias") ;
    Names.append("Jacob") ;
    Names.append("Jaha") ;
    Names.append("Mikel") ;
    Names.append("Kirk") ;
    Names.append("Giles") ;
    Names.append("Willian") ;
    Names.append("Rory") ;
    Names.append("Grover") ;
    Names.append("Kristopher") ;
    Names.append("Chad") ;
    Names.append("Bruce") ;
    Names.append("Chung") ;
    Names.append("Anderson") ;
    Names.append("Forest") ;
    Names.append("Jamie") ;
    Names.append("Lonnie") ;
    Names.append("James") ;
    Names.append("Noe") ;
    Names.append("Joe") ;

    WormFileNameL = ":/worms/Textures/Worms/WormL_1.bmp";
    WormFileNameR = ":/worms/Textures/Worms/WormR_1.bmp";

    ApplyTexture(1);

}

void Worm::ApplyTexture(int direction) // 1 - Left | 2 - Right | 3 - Jump | false = Left | True = right
{

    if ( direction == 1)
    {
        WormTexture.load(WormFileNameL);
        Direction = false;
    }
    else if (direction == 2)
    {
        WormTexture.load(WormFileNameR);
        Direction = true;
    }
    else
        return;

    this->setMinimumHeight(WormTexture.height());
    this->setMaximumHeight(WormTexture.height());
    this->setMinimumWidth(WormTexture.width());
    this->setMaximumWidth(WormTexture.width());
    this->setPixmap(QPixmap::fromImage(WormTexture));
    this->show();
}

void Worm::DisplayHealth()
{
    HealthDisplay->setText(QString::number(Health));
    HealthDisplay->adjustSize();
    HealthDisplay->move(x(),y()-HealthDisplay->height());
    HealthDisplay->show();

    NameDisplay->setText(Name);
    NameDisplay->adjustSize();
    NameDisplay->move(x(),HealthDisplay->y()-NameDisplay->height());
    NameDisplay->show();
}
