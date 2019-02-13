#ifndef GUNLISTDISPLAY_H
#define GUNLISTDISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

#include "mylabel.h"

class GunListDisplay : public QWidget
{
    Q_OBJECT
public:
    GunListDisplay(QWidget *parent = 0);

private:
    QLabel *GunListDispaly;

public slots:
    void ShowGunList();
    void HideGunList();
    void GunTypeProxy(QString N);

signals:
    void setGunType(QString N);

};

#endif // GUNLISTDISPLAY_H
