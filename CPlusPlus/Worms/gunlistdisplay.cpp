#include "gunlistdisplay.h"

GunListDisplay::GunListDisplay(QWidget *parent)
{
    setParent(parent);

    GunListDispaly = new QLabel (parent);
    GunListDispaly->setMinimumHeight(320);
    GunListDispaly->setMinimumWidth(155);
    GunListDispaly->move(parent->width()/2 - GunListDispaly->width()/2
                       , parent->height()/2 - GunListDispaly->height()/2);
    GunListDispaly->show();
    GunListDispaly->setVisible(false);

    myLabel *Punch = new myLabel (GunListDispaly);
    Punch->setName("Punch");
    Punch->setTexture();
    myLabel *Pistol = new myLabel (GunListDispaly);
    Pistol->setName("Pistol");
    Pistol->setTexture();
    myLabel *RocketLauncher = new myLabel (GunListDispaly);
    RocketLauncher->setName("RocketLauncher");
    RocketLauncher->setTexture();
    myLabel *Grenade = new myLabel (GunListDispaly);
    Grenade->setName("Grenade");
    Grenade->setTexture();
    myLabel *Airstrike = new myLabel (GunListDispaly);
    Airstrike->setName("Airstrike");
    Airstrike->setTexture();
    myLabel *Teleport = new myLabel (GunListDispaly);
    Teleport->setName("Teleport");
    Teleport->setTexture();

    QGridLayout *gunlayout = new QGridLayout();
    gunlayout->addWidget(Pistol,0,0,1,1);
    gunlayout->addWidget(RocketLauncher,0,1,1,1);
    gunlayout->addWidget(Grenade,1,0,1,1);
    gunlayout->addWidget(Punch,1,1,1,1);
    gunlayout->addWidget(Airstrike,2,0,1,1);
    gunlayout->addWidget(Teleport,2,1,1,1);
    GunListDispaly->setLayout(gunlayout);

    connect(Punch,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));
    connect(Pistol,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));
    connect(RocketLauncher,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));
//    connect(Grenade,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));
    connect(Airstrike,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));
    connect(Teleport,SIGNAL(clicked(QString)),this,SLOT(GunTypeProxy(QString)));

}

void GunListDisplay::ShowGunList()
{
    GunListDispaly->setVisible(true);
    GunListDispaly->raise();
}

void GunListDisplay::HideGunList()
{
    GunListDispaly->setVisible(false);
}

void GunListDisplay::GunTypeProxy(QString N)
{
    emit setGunType(N);
    emit HideGunList();
}
