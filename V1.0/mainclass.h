#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QList>
#include <QVector>
#include <QLabel>
#include <QTimer>
#include <QVector>
#include <QInputDialog>
#include <QRect>
#include <QDesktopWidget>
#include <QMovie>
#include <QFont>
#include <QTime>
#include <QtNetwork/QUdpSocket>

#include "worm.h"
#include "bullet.h"
#include "player.h"
#include "keyhandler.h"
#include "collisiondetection.h"
#include "QDirIterator"
#include "gunlistdisplay.h"
#include "mouseeventhandler.h"
#include "maplabel.h"

namespace Ui {
class MainClass;
}

class MainClass : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainClass(QWidget *parent = 0);
    ~MainClass();

private:
    Ui::MainClass *ui;
    //--------------------FUNCTIONS-------------------------//
    void GenerateMap();
    void DisplayBackground();
    void SpawnPlayers(int AmountPlayers , int WormsTeam);
    void StartGame();
    void UpdateWormPositions(QByteArray datagram);
    void UpdateWormNames(QByteArray datagram);
    QByteArray ConstructWormPositiondatagram();
    QByteArray ConstructWormNamedatagram();
    //--------------------CONTAINERS-------------------------//
    QList<Player*> *players;
    QList<Bullet*> *bullets;
    QList<MapLabel*> maps;
    QVector<QVector<int>> ArrMap;
    QVector<QVector<int>> TurnSuccession;
    //------------------CLASSVARIABLES-----------------------//
    QLabel *MapDisplay  , *BackGroundDisplay , *WinnerDisplay, *displayFireWorks , *displayExplosion ;
    QString MapFileName , BackgroundFilename , GunName       , MyName            , EnemyName , MapName , Dir ;
    QTimer *GeneralTimer, *WaitingPlayer     , *WaitingAck ;
    QMovie *FireWorks   , *Explosion;
    QImage  Map         , BackGround;
    Worm   *ActiveWorm;
    Player *ActivePlayer;
    QTime  *Time;
    QLCDNumber  *TimeRemaining;
    QHostAddress MyIP , EnemyIP;
    QByteArray   Datagram;
    QUdpSocket  *socket;
    //___CHILDREN___//
    CollisionDetection *collisiondetection;
    KeyHandler *keyhandler;
    //--------------------VARIABLES--------------------------//
    const int RefreshRate = 25;    
    double UniversalGravity;

    int TurnTime   , Counter       , TimeAmount     , CurrentTime   , TimeHolder
      , TurnAmounts, TurnCounter   , GrenadeCounter , deletedWorm   , deletedPlayersWorm
      , movieTimer , AmountofWorms , AmountofPlayers;

    bool EndTurn , GunTypeSet  , GrenadeSpawned  , TurnSet , MovieStarted , SinglePlayer
       , Host    , Multiplayer , WormFreeFalling , MyTurn  , ShotTaken    , Client  ;

private slots:

    void UpdateMap(QImage map);
    void UpdateDisplay();
    void TimerFinished();
    void EndmyTurn();
    void setGynType(QString N);
    void StopAnimation();
    void SpawnBullet(int Positionx , int Positiony , double strengh);
    void WormDied(Player *p , QList<Worm*> *ws , Worm *w);
    void WormDied(int IndexPlayer , int IndexWorm);
    void changBulletAngle(Bullet *b);
    void receiveData();
    void WaitingPlayersFired();
    void WaitingAckFired();
    void MultiplayerTurnChange();
    void MapLabelClicked(QString filename ,QString name);
    void on_BSinglePlayer_clicked();
    void on_BNext_clicked();
    void on_BMUltiPlayer_clicked();
    void on_BHost_clicked();
    void on_BJoinGame_clicked();
    void on_BStartGame_clicked();
    void on_BJoin_clicked();

signals:
    void UpdateActiveWorm(Worm *w);
    void ActivateMouseHandler(bool PBStatus);
};

#endif // MAINCLASS_H
