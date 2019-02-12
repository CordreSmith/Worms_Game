#include "mylabel.h"
#include <QPixmap>
#include <QDebug>

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::setName(QString N)
{
    Name = N;
}

void myLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(Name);
}

void myLabel::setTexture()
{
    if (Name == "Punch")
    {
        FileName = ":/guns/Textures/Guns/ListImages/Punch.png";
    }
    else if (Name == "Pistol")
    {
        FileName = ":/guns/Textures/Guns/ListImages/Psitol.png";
    }
    else if (Name == "Grenade")
    {
        FileName = ":/guns/Textures/Guns/ListImages/Grenade.png";
    }
    else if (Name == "Airstrike")
    {
        FileName = ":/guns/Textures/Guns/ListImages/Airstrike.png";
    }
    else if (Name == "RocketLauncher")
    {
        FileName = ":/guns/Textures/Guns/ListImages/RocketLauncher.png";
    }
    else if (Name == "Teleport")
    {
        FileName = ":/guns/Textures/Guns/ListImages/Teleport.png";
    }

    GunTexture.load(FileName);

    this->setText(Name);
    this->setMinimumHeight(75);
    this->setMinimumWidth(75);
    this->setMaximumHeight(75);
    this->setMaximumWidth(75);
    this->setScaledContents(true);
    this->setToolTip(Name);
    this->setToolTipDuration(1500);
    this->setPixmap(QPixmap::fromImage(GunTexture));
    this->show();
}
