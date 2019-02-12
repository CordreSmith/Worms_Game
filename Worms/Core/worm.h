#ifndef WORM_H
#define WORM_H

#include <QLabel>
#include <QString>
#include <QList>

class Worm : public QLabel
{
    Q_OBJECT
public:
    explicit Worm(QWidget *parent = 0);

    void ApplyTexture(int direction);
    void DisplayHealth();

    bool Left ,Right ,Direction ,Jump ,Active ,onGround;
    int Health;
    QString Name , WormFileNameL , WormFileNameR;
    double ySpeed , xSpeed;
    QList<QString> Names;
    QLabel *HealthDisplay , *NameDisplay;
    QImage WormTexture;


};

#endif // WORM_H
