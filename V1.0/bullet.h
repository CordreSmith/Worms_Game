#ifndef BULLET_H
#define BULLET_H

#include <QLabel>
#include <QImage>
#include <QPixmap>

class Bullet : public QLabel
{
public:
    explicit Bullet(QWidget * parent = 0);

    void setxSpeed(double speed);
    void setySpeed(double speed);
    void setBounceFalse();
    void setPistol();
    void setRocketLauncher();
    void setGrenade();
    void setPunch();
    void setAirstrike();
    void getTexture();
    double getxSpeed ();
    double getySpeed ();
    double getmassEffect();
    int getDamage ();
    int getAoE();
    bool getGravityEffect();
    bool getWormHit();
    bool getBounce();
    bool getExplosion();    
    QString getFileName();

private:    
    //---------------FUNCTIONS---------------//
    void setTexture();
    //---------------VARIABLES---------------//
    bool   Bounce , Punch  , EffectedbyGravity , CanHitWorm , Explosion;
    double xSpeed , ySpeed , massaEffect;
    int    Damage , AoE;
    //------------CLASSVARIABLES-------------//
    QImage  Texture;
    QString FileName;

};

#endif // BULLET_H
