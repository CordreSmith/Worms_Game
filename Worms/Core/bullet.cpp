#include "bullet.h"


Bullet::Bullet(QWidget *parent) : QLabel(parent)
{
    Punch = false;
    xSpeed = 0;
    ySpeed = 0;
    Explosion = false;
}

void Bullet::setTexture()
{
    this->setMinimumHeight(Texture.height());
    this->setMinimumWidth(Texture.width());
    this->setMaximumHeight(Texture.height());
    this->setMaximumWidth(Texture.width());
    this->setPixmap(QPixmap::fromImage(Texture));
    this->show();
}

void Bullet::setxSpeed(double speed)
{
    xSpeed = speed;
}

double Bullet::getxSpeed()
{
    return (xSpeed);
}

void Bullet::setySpeed(double speed)
{
    ySpeed = speed;
}
double Bullet::getySpeed()
{
    return(ySpeed);
}

int Bullet::getDamage()
{
    return(Damage);
}

double Bullet::getmassEffect()
{
    return(massaEffect);
}

bool Bullet::getGravityEffect()
{
    return(EffectedbyGravity);
}

bool Bullet::getWormHit()
{
    return(CanHitWorm);
}

bool Bullet::getBounce()
{
    return(Bounce);
}

bool Bullet::getExplosion()
{
    return(Explosion);
}

void Bullet::setBounceFalse()
{
    Bounce = false;
}

QString Bullet::getFileName()
{
    return(FileName);
}

int Bullet::getAoE()
{
    return(AoE);
}

void Bullet::setPistol()
{
    FileName = ":/bullets/Textures/Bullets/Pistol.png";
    Damage = 15;
    AoE = 1;
    EffectedbyGravity = false;
    CanHitWorm = true;
    Bounce = false;
    Texture.load(FileName);
    setTexture();
}

void Bullet::setRocketLauncher()
{

    FileName = ":/bullets/Textures/Bullets/Rocket.png";
    Damage = 35;
    AoE = 25;
    massaEffect = 0.45;
    EffectedbyGravity = true;
    CanHitWorm = true;
    Bounce = false;
    Explosion = true;
    Texture.load(FileName);
    setTexture();
}

void Bullet::setGrenade()
{
    FileName = ":/bullets/Textures/Bullets/Grenade.png";
    Damage = 50;
    AoE = 30;
    massaEffect = 0.2;
    EffectedbyGravity = true;
    CanHitWorm = false;
    Bounce = true;
    Explosion = true;

    Texture.load(FileName);
    setTexture();
}

void Bullet::setPunch()
{
    Damage = 30;
    Punch = true;
}

void Bullet::setAirstrike()
{
    FileName = ":/bullets/Textures/Bullets/Airstrike.png";
    Damage = 40;
    AoE = 40;
    massaEffect = 0;
    EffectedbyGravity = true;
    CanHitWorm = false;
    Bounce = false;
    Explosion = true;

    Texture.load(FileName);
    setTexture();
}


