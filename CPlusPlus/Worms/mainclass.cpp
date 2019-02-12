#include "mainclass.h"
#include "ui_mainclass.h"

#include <QDebug>

MainClass::MainClass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClass)
{
    ui->setupUi(this);

    Dir = "E://Documents//NWU//3de Jaar//EERI 316//QT//Game//Worms_Main_Game//Textures//";

    // Setup HomeScreen

    MyName = QInputDialog::getText(this,tr("Master's Name"),tr("Please provide your name, Master : "));

    resize(300,300);
    ui->LWelcome->setText(ui->LWelcome->text() + MyName);
    ui->HomePage->move(this->width()/2 - ui->HomePage->width()/2,this->height()/2 - ui->HomePage->height()/2);
    ui->HomePage->show();
    ui->InfoPage->hide();
    ui->MultiPlayerPage->hide();
    ui->LobbyPage->hide();
    ui->JoinPage->hide();
    ui->BStartGame->setEnabled(false);

    //  Network Initialize //

    SinglePlayer    = false;
    Multiplayer     = false;
    Host            = false;
    Client          = false;
    MyTurn          = false;
    WormFreeFalling = true;

}

MainClass::~MainClass()
{
    delete ui;
}

void MainClass::GenerateMap()
{
    Map.load(MapFileName);
    Map = Map.convertToFormat( QImage::Format_ARGB32 );
    ArrMap.resize(Map.width());
    for ( int j = 0 ; j <= Map.width() - 1; j++ )
    {
        ArrMap[j].resize(Map.height());
        for ( int i = 0 ; i <= Map.height() - 1 ; i++ )
        {
            QRgb TempcolorRgb = Map.pixel( j,i );
            QColor Tempcolor (TempcolorRgb);

            if (Tempcolor.red() == 34 && Tempcolor.blue() == 76 && Tempcolor.green() == 177 )
                ArrMap[j][i] = 1;
            else
            {
                ArrMap[j][i] = 0;
                QRgb colTransparent = qRgba(Tempcolor.red(),Tempcolor.green(),Tempcolor.blue(),0);
                Map.setPixel(j,i,colTransparent);
            }
        }
    }
    resize(Map.width(),Map.height());
    MapDisplay = new QLabel (this);
    MapDisplay->setPixmap(QPixmap::fromImage(Map));
    MapDisplay->show();
    MapDisplay->setMinimumHeight(Map.height());
    MapDisplay->setMinimumWidth(Map.width());
    MapDisplay->raise();
}

void MainClass::DisplayBackground()
{
    BackgroundFilename =  Dir + "Backgrounds//BackGround1.bmp";
    BackGround.load(BackgroundFilename);

    BackGroundDisplay = new QLabel (this);
    BackGroundDisplay->setPixmap(QPixmap::fromImage(BackGround));
    BackGroundDisplay->show();
    BackGroundDisplay->setMinimumHeight(BackGround.height());
    BackGroundDisplay->setMaximumHeight(BackGround.height());
    BackGroundDisplay->setMinimumWidth(BackGround.width());
    BackGroundDisplay->setMaximumWidth(BackGround.width());
    BackGroundDisplay->lower();
}

void MainClass::SpawnPlayers(int AmountPlayers, int WormsTeam)
{
    for (int i = 1 ; i <= AmountPlayers ; i++)
    {
        Player *player = new Player (this);
        if (i == 1 && Multiplayer == false)
            player->setName(MyName);
        else if (Multiplayer == true && i == 1)
        {
            if (Host == true)
                player->setName(MyName);
            else
                player->setName(EnemyName);
        }
        else if (Multiplayer == true && i == 2)
        {
            if (Host == true)
                player->setName(EnemyName);
            else
                player->setName(MyName);
        }
        else
            player->setName("Player" + QString::number(i));

        player->setNumber(i);
        player->spawnWorms(WormsTeam);
        players->append(player);
        QList<Worm*> *worms = player->returnWormList();

        if (SinglePlayer == true)
        {
            if (i == 1)
            {
                player->setActive();
                ActivePlayer = player;
                ActiveWorm = worms->value(0);
                worms->value(0)->Active = true;
                emit UpdateActiveWorm(ActiveWorm);
            }
        }
        else if ( Multiplayer == true)
        {
            if (i == 1 && Host == true)
            {
                player->setActive();
                ActivePlayer = player;
                ActiveWorm = worms->value(0);
                worms->value(0)->Active = true;
                emit UpdateActiveWorm(ActiveWorm);
            }
        }
        if (worms->isEmpty() == false)
        {
            QListIterator<Worm*> WormIter(*worms);
            while (WormIter.hasNext())
            {
                Worm *worm = WormIter.next();
                int Ranx = qrand()%(Map.width()-20);
                int Rany = qrand()%(Map.height()-20);
                int RanName = qrand()%(20);
                worm->Name = worm->Names[RanName];
                int ClosestDown = 0;
                int ClosestUp = 0;
                for (int i = Rany + 1 ; i < Map.height() ; i++)
                {
                    if (ArrMap[Ranx][i] == 1 && ArrMap[Ranx][i-1] == 0)
                    {
                       ClosestDown = i;
                       break;
                    }
                }
                for (int j = 1 ; j < Rany ; j++)
                {
                    if (ArrMap[Ranx][j] == 1 && ArrMap[Ranx][j-1] == 0)
                    {
                       ClosestUp = j;
                       break;
                    }
                }
                if (ClosestDown - ClosestUp > 0)
                {
                    worm->move(Ranx , ClosestDown-worm->height());
                    worm->DisplayHealth();
                }
                else
                {
                    worm->move(Ranx , ClosestUp-worm->height());
                    worm->DisplayHealth();
                }
            }
        }
    }

    if (SinglePlayer == true)
    {
        TurnSuccession.resize(WormsTeam*AmountPlayers);
        for (int i = 0 ; i < WormsTeam ; i++)
        {
            for (int j = 0 ; j < AmountPlayers ; j++)
            {
                TurnSuccession[TurnAmounts].resize(2);
                TurnSuccession[TurnAmounts][0] = j;
                TurnSuccession[TurnAmounts][1] = i;
                TurnAmounts++;
            }
        }
    }
}

void MainClass::StartGame()
{
    TimeRemaining = new QLCDNumber(2);
    WinnerDisplay = new QLabel(this);
    GeneralTimer  = new QTimer (this);
    keyhandler = new KeyHandler (this) ;
    FireWorks  = new QMovie( Dir + "Gifs//Fireworks2.gif");
    Explosion  = new QMovie( Dir + "Gifs//Explosion2.gif");
    players = new QList<Player*>();
    bullets = new QList<Bullet*>();
    Time    = new QTime();

    TurnAmounts    = 0;
    TurnCounter    = 0;
    Counter        = 0;
    GrenadeCounter = 0;
    TimeHolder     = 0;
    movieTimer     = 0;
    TurnTime       = 30;
    MovieStarted   = false;
    EndTurn        = false;
    GunTypeSet     = false;
    GrenadeSpawned = false;
    ShotTaken      = false;

    QFont myFont("Times" , 30 , QFont::Bold);

    UniversalGravity = 10 / (1000 / RefreshRate);
    TimeAmount = round (TurnTime*1000 / RefreshRate) ;
    CurrentTime  = TurnTime;

    Time->start();
    qsrand(Time->msec() * 100);

    GeneralTimer->setInterval(RefreshRate);
    displayFireWorks = new QLabel(this);
    displayFireWorks->setMovie(FireWorks);
    displayFireWorks->setVisible(false);
    displayExplosion = new QLabel(this);
    displayExplosion->setMovie(Explosion);
    displayExplosion->setVisible(false);    
    TimeRemaining->setParent(this);
    TimeRemaining->setSegmentStyle(QLCDNumber::Outline);
    TimeRemaining->setFrameStyle(QFrame::NoFrame);
    TimeRemaining->move(20,20);
    TimeRemaining->setMaximumHeight(50);
    TimeRemaining->setMaximumWidth(50);
    TimeRemaining->setPalette(Qt::black);
    TimeRemaining->display(CurrentTime);
    TimeRemaining->show(); 
    WinnerDisplay->setStyleSheet("color : red");
    WinnerDisplay->setFont(myFont);
    WinnerDisplay->setVisible(false);

    connect(this,SIGNAL(UpdateActiveWorm(Worm*)),keyhandler,SLOT(ActiveWormUpdated(Worm*)));
    connect(GeneralTimer,SIGNAL(timeout()),this,SLOT(TimerFinished()));

    DisplayBackground();
    GenerateMap();
    SpawnPlayers(AmountofPlayers,AmountofWorms);

    MouseEventHandler *mousehandler = new MouseEventHandler (this);
    GunListDisplay *gunlistdisplay = new GunListDisplay (this);
    collisiondetection = new CollisionDetection (this) ;

    connect(keyhandler,SIGNAL(EndTurn()),this,SLOT(EndmyTurn()));
    connect(keyhandler,SIGNAL(GunListShow()),gunlistdisplay,SLOT(ShowGunList()));
    connect(gunlistdisplay,SIGNAL(setGunType(QString)),this,SLOT(setGynType(QString)));
    connect(mousehandler,SIGNAL(SpawnBullet(int,int,double)),this,SLOT(SpawnBullet(int,int,double)));
    connect(this,SIGNAL(ActivateMouseHandler(bool)),mousehandler,SLOT(activate(bool)));
    connect(collisiondetection,SIGNAL(updateMap(QImage)),this,SLOT(UpdateMap(QImage)));
    connect(collisiondetection,SIGNAL(WormOutofBounds(Player*,QList<Worm*>*,Worm*)),this,SLOT(WormDied(Player*,QList<Worm*>*,Worm*)));

    collisiondetection->setValues(ArrMap,players,Map);

    if (SinglePlayer == true)
        GeneralTimer->start();
}

void MainClass::UpdateWormPositions(QByteArray datagram)
{
    QList<QByteArray> Players , WormsP1 , WormsP2;
    int PlayerNum , WormNum ;
    PlayerNum = 0;
    WormNum   = 0;
    Players = datagram.split('#');
    WormsP1 = Players[0].split(';');
    WormsP2 = Players[1].split(';');

    QListIterator<Player*> PlayerIter(*players);
    while(PlayerIter.hasNext())
    {
        PlayerNum ++;
        WormNum = 0;
        Player *player = PlayerIter.next();
        QList<Worm*> *worms = player->returnWormList();
        QListIterator<Worm*> WormIter(*worms);
        while(WormIter.hasNext())
        {
            Worm *worm = WormIter.next();
            if (PlayerNum == 1)
            {
                QList<QByteArray> position;
                position = WormsP1[WormNum].split(':');
                if ((worm->x() - position[0].toInt()) < 0)
                    worm->ApplyTexture(2);
                else
                    worm->ApplyTexture(1);

                worm->move(position[0].toInt() , position[1].toInt());
                worm->Health = position[2].toInt();
                worm->DisplayHealth();
            }
            else if (PlayerNum == 2)
            {
                QList<QByteArray> position;
                position = WormsP2[WormNum].split(':');
                if ((worm->x() - position[0].toInt()) < 0)
                    worm->ApplyTexture(2);
                else
                    worm->ApplyTexture(1);

                worm->move(position[0].toInt() , position[1].toInt());
                worm->Health = position[2].toInt();
                worm->DisplayHealth();
            }
            WormNum++;
        }
    }


}

void MainClass::UpdateWormNames(QByteArray datagram)
{
    QList<QByteArray> Players , WormsP1 , WormsP2;
    int PlayerNum , WormNum ;
    PlayerNum = 0;
    WormNum   = 0;

    Players = datagram.split('#');
    WormsP1 = Players[0].split(';');
    WormsP2 = Players[1].split(';');

    QListIterator<Player*> PlayerIter(*players);
    while(PlayerIter.hasNext())
    {
        PlayerNum ++;
        WormNum = 0;
        Player *player = PlayerIter.next();
        QList<Worm*> *worms = player->returnWormList();
        QListIterator<Worm*> WormIter(*worms);
        while(WormIter.hasNext())
        {
            Worm *worm = WormIter.next();
            if (PlayerNum == 1)
            {
                worm->Name = WormsP1[WormNum];
                worm->DisplayHealth();
            }
            else if (PlayerNum == 2)
            {
                worm->Name = WormsP2[WormNum];
                worm->DisplayHealth();
            }
            WormNum++;
        }
    }
}

QByteArray MainClass::ConstructWormPositiondatagram()
{
    QByteArray temp;
    temp.clear();

    QListIterator<Player*> PlayerIter(*players);
    while(PlayerIter.hasNext())
    {
        Player *player = PlayerIter.next();
        QList<Worm*> *worms = player->returnWormList();
        QListIterator<Worm*> WormIter(*worms);
        while(WormIter.hasNext())
        {
            Worm *worm = WormIter.next();
            temp = temp + QByteArray::number(worm->x()) + ':' + QByteArray::number(worm->y()) + ':' + QByteArray::number(worm->Health);
            if (WormIter.hasNext() == true)
                temp = temp + ';' ;
        }
        if (PlayerIter.hasNext() == true)
            temp = temp + '#' ;
    }
    return temp;
}

QByteArray MainClass::ConstructWormNamedatagram()
{
    QByteArray temp;
    temp.clear();

    QListIterator<Player*> PlayerIter(*players);
    while(PlayerIter.hasNext())
    {
        Player *player = PlayerIter.next();
        QList<Worm*> *worms = player->returnWormList();
        QListIterator<Worm*> WormIter(*worms);
        while(WormIter.hasNext())
        {
            Worm *worm = WormIter.next();
            temp = temp + worm->Name.toUtf8();
            if (WormIter.hasNext() == true)
                temp = temp + ';' ;
        }
        if (PlayerIter.hasNext() == true)
            temp = temp + '#' ;
    }
    return temp;
}

void MainClass::UpdateMap(QImage map)
{
    Map = map;
    MapDisplay->setPixmap(QPixmap::fromImage(map));
    MapDisplay->show();
}

void MainClass::UpdateDisplay()
{
        Counter++;
        TimeHolder++;

        if (TimeHolder == 40)
        {
            CurrentTime --;
            TimeRemaining->display(CurrentTime);
            TimeHolder = 0;
        }

        //-----------------WORM FREEFALL DETECTION--------------------//
        if (players->isEmpty() == false)
        {
            QListIterator<Player*> PlayerIter(*players);
            while (PlayerIter.hasNext())
            {
                Player *player = PlayerIter.next();
                if (players->size() == 1)
                {
                    GeneralTimer->stop();
                    displayFireWorks->resize(Map.width(),Map.height());
                    displayFireWorks->setScaledContents(true);
                    displayFireWorks->setVisible(true);
                    displayFireWorks->raise();
                    FireWorks->start();
                    WinnerDisplay->setText("The winner is " + player->getName() + " !");
                    WinnerDisplay->adjustSize();
                    WinnerDisplay->move(Map.width()/2 - WinnerDisplay->width()/2 ,
                                        Map.height()/2 - WinnerDisplay->height()/2);
                    WinnerDisplay->setVisible(true);
                    WinnerDisplay->raise();
                    if (Multiplayer == true)
                    {
                        Datagram = "#Done" + player->getName().toUtf8();
                        socket->writeDatagram(Datagram,EnemyIP,2356);
                        WaitingAck->start();
                    }
                    return;
                }
                else
                {
                    QList<Worm*> *worms = player->returnWormList();
                    QListIterator<Worm*> WormIter(*worms);
                    while (WormIter.hasNext())
                    {
                        Worm *worm = WormIter.next();
                        if (worm->Health <= 0)
                            emit WormDied(player,worms,worm);
                        else
                        {
                            collisiondetection->freefallDetection(worm);
                            if (worm->onGround == false)
                            {
                                if (collisiondetection->onMap(worm) == false)
                                    emit WormDied(player,worms,worm);
                                else
                                {
                                    collisiondetection->freefallCollision(worm);
                                    if (worm->onGround == false)
                                    {
                                        worm->ySpeed = worm->ySpeed + 0.1;
                                        worm->move(worm->x()+worm->xSpeed,worm->y()+worm->ySpeed);
                                        worm->DisplayHealth();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        //-----------------WORM MOVEMENT--------------------//

        if (ActiveWorm->Left == true)
        {
            if (collisiondetection->movementCollision(ActiveWorm) == 0)
                return;
            else if (collisiondetection->movementCollision(ActiveWorm) == 1)
            {
                ActiveWorm->move(ActiveWorm->x()-1,ActiveWorm->y()-1);
                ActiveWorm->DisplayHealth();
            }
            else if (collisiondetection->movementCollision(ActiveWorm) == 2)
            {
                ActiveWorm->move(ActiveWorm->x()-1,ActiveWorm->y());
                ActiveWorm->DisplayHealth();
            }
            if (Multiplayer == true)
            {
                Datagram = "#Worm" + ConstructWormPositiondatagram();
                socket->writeDatagram(Datagram,EnemyIP,2356);
            }
        }
        else if (ActiveWorm->Right == true)
        {
            if (collisiondetection->movementCollision(ActiveWorm) == 0)
                return;
            else if (collisiondetection->movementCollision(ActiveWorm) == 1)
            {
                ActiveWorm->move(ActiveWorm->x()+1,ActiveWorm->y()-1);
                ActiveWorm->DisplayHealth();
            }
            else if (collisiondetection->movementCollision(ActiveWorm) == 2)
            {
                ActiveWorm->move(ActiveWorm->x()+1,ActiveWorm->y());
                ActiveWorm->DisplayHealth();
            }
            if (Multiplayer == true)
            {
                Datagram = "#Worm" + ConstructWormPositiondatagram();
                socket->writeDatagram(Datagram,EnemyIP,2356);
            }
        }
        if (ActiveWorm->Jump == true)
        {
            if (ActiveWorm->onGround == true)
            {
                ActiveWorm->ySpeed = -3;
                ActiveWorm->onGround = false;
                ActiveWorm->DisplayHealth();
            }
            if (Multiplayer == true)
            {
                Datagram = "#Worm" + ConstructWormPositiondatagram();
                socket->writeDatagram(Datagram,EnemyIP,2356);
            }
        }

        //-----------------BULLET MOVEMENT--------------------//

        if (bullets->isEmpty() == false)
        {
            QListIterator<Bullet*> BulletIter(*bullets);
            while (BulletIter.hasNext())
            {
                Bullet *bullet = BulletIter.next();
                if (bullet->x() < 0 || bullet->x() > Map.width())
                {
                    bullets->removeAt(bullets->indexOf(bullet));
                    bullet->deleteLater();
                }
                if (collisiondetection->bulletWormCollision(bullet) == true)
                {
                    if (bullet->getExplosion() == true)
                    {
                        displayExplosion->setVisible(true);
                        displayExplosion->setMinimumSize(bullet->getAoE() *4 , bullet->getAoE() *4);
                        displayExplosion->setMaximumSize(bullet->getAoE() *4 , bullet->getAoE() *4);
                        displayExplosion->setScaledContents(true);
                        displayExplosion->move(bullet->x()-displayExplosion->width()/2,bullet->y()-displayExplosion->height()/2);
                        displayExplosion->raise();
                        Explosion->start();
                        QTimer::singleShot(700,this,SLOT(StopAnimation()));
                    }
                    if (Multiplayer == true)
                    {
                        Datagram = "#MapD" + QByteArray::number(bullet->x()+(bullet->width()/2)) + ':' + QByteArray::number(bullet->y()+(bullet->height()/2)) + ':' + QByteArray::number(bullet->getAoE());
                        socket->writeDatagram(Datagram,EnemyIP,2356);
                        WaitingAck->start();
                    }
                    bullets->removeAt(bullets->indexOf(bullet));
                    bullet->deleteLater();
                }
                else if (collisiondetection->bulletTerrainCollision(bullet) == true)
                {
                    if (bullet->getExplosion() == true)
                    {
                        displayExplosion->setVisible(true);
                        displayExplosion->setMinimumSize(bullet->getAoE() *4 , bullet->getAoE() *4);
                        displayExplosion->setMaximumSize(bullet->getAoE() *4 , bullet->getAoE() *4);
                        displayExplosion->setScaledContents(true);
                        displayExplosion->move(bullet->x()-displayExplosion->width()/2,bullet->y()-displayExplosion->height()/2);
                        displayExplosion->raise();
                        Explosion->start();
                        QTimer::singleShot(700,this,SLOT(StopAnimation()));
                    }
                    if (Multiplayer == true)
                    {
                        Datagram = "#MapD" + QByteArray::number(bullet->x()+(bullet->width()/2)) + ':' + QByteArray::number(bullet->y()+(bullet->height()/2)) + ':' + QByteArray::number(bullet->getAoE());
                        socket->writeDatagram(Datagram,EnemyIP,2356);
                        WaitingAck->start();
                    }
                    bullets->removeAt(bullets->indexOf(bullet));
                    bullet->deleteLater();
                }
                else
                {
                    if (bullet->getGravityEffect() == true)
                        bullet->setySpeed(bullet->getySpeed() + 0.05 );
                    bullet->setxSpeed(bullet->getxSpeed()-0.00001);
                    bullet->move(bullet->x()+bullet->getxSpeed() ,
                                 bullet->y()+bullet->getySpeed());
                    changBulletAngle(bullet);
                }
            }
        }
        else if (bullets->isEmpty() == true && ShotTaken == true)
            emit EndmyTurn();

        //-----------------TURNHANDLER SINGLEPLAYER--------------------//

        if ( SinglePlayer == true)
        {
            if (Counter > TimeAmount || EndTurn == true)
            {
                CurrentTime = TurnTime;
                TimeRemaining->display(CurrentTime);
                EndTurn = false;
                GunTypeSet = false;
                ShotTaken = false;
                if (TurnAmounts > 1)
                {
                    Player *playerNA = players->at(TurnSuccession[TurnCounter][0]);
                    playerNA->setNotActive();
                    QList<Worm*> *wormsNA = playerNA->returnWormList();
                    Worm *wormNA = wormsNA->at(TurnSuccession[TurnCounter][1]);
                    wormNA->Active = false;

                    TurnCounter++;
                    if (TurnCounter >= TurnAmounts)
                        TurnCounter = 0;

                    Player *playerA = players->at(TurnSuccession[TurnCounter][0]);
                    playerA->setActive();
                    ActivePlayer = playerA;
                    QList<Worm*> *wormsA = playerA->returnWormList();
                    Worm *wormA = wormsA->at(TurnSuccession[TurnCounter][1]);
                    wormA->Active = true;
                    ActiveWorm = wormA;
                    emit UpdateActiveWorm(ActiveWorm);
                }
                Counter = 0;
            }
        }

        //-----------------TURNHANDLER MULTIPLAYER--------------------//
        else if ( Multiplayer == true )
        {
            if ((Counter > TimeAmount || EndTurn == true))
            {
                bool Temp = false;
                QListIterator<Player*> PlayerIter(*players);
                while (PlayerIter.hasNext())
                {
                    Player *player = PlayerIter.next();
                    QList<Worm*> *worms = player->returnWormList();
                    QListIterator<Worm*> WormIter(*worms);
                    while (WormIter.hasNext())
                    {
                        Worm *worm = WormIter.next();
                        collisiondetection->freefallDetection(worm);
                        if (worm->onGround == false)
                            Temp = true;
                        if (worm->xSpeed != 0 || worm->ySpeed != 0)
                            Temp = true;
                    }
                }

                if (Temp == false)
                    WormFreeFalling = false;
                if (WormFreeFalling == false)
                {
                    MyTurn          = false;
                    EndTurn         = false;
                    ShotTaken       = false;
                    GunTypeSet      = false;
                    WormFreeFalling = true;
                    Counter = 0;

                    GeneralTimer->stop();
                    keyhandler->setEnabled(false);
                    TimeRemaining->display(TurnTime);
                    CurrentTime = TurnTime;
                    QList<Worm*> *worms = ActivePlayer->returnWormList();
                    Datagram = "#Your" + QByteArray::number(worms->indexOf(ActiveWorm));
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                    WaitingAck->start();
                    ActivePlayer->setNotActive();
                    ActiveWorm->Active = false;
                }
            }
        }
}

void MainClass::TimerFinished()
{
    if (SinglePlayer == true)
        emit UpdateDisplay();

    else if (Multiplayer == true)
    {
        if (MyTurn == true)
            UpdateDisplay();
        else if (MyTurn == false)
            GeneralTimer->stop();
            Counter = 0;
            TimeHolder = 0;
    }
}

void MainClass::EndmyTurn()
{
    EndTurn = true;
}

void MainClass::setGynType(QString N)
{
    GunName = N;
    GunTypeSet = true;
    if (N == "Teleport" || N == "Airstrike" || N == "Pistol")
        emit ActivateMouseHandler(false);
    else
        emit ActivateMouseHandler(true);
}

void MainClass::StopAnimation()
{
    Explosion->stop();
    displayExplosion->hide();
}

void MainClass::SpawnBullet(int Positionx, int Positiony, double strengh)
{
    if (GunTypeSet == true && ShotTaken == false)
    {
        double wormcenterx = ActiveWorm->x() + ActiveWorm->width()/2;
        double wormcentery = ActiveWorm->y() + ActiveWorm->height()/2;
        double delthax = double(Positionx) - wormcenterx ;
        double delthay = double(Positiony) - wormcentery ;
        double angle = atan2(delthay,delthax);

        double projectilexSpeed = cos(angle) * strengh;
        double projectileySpeed = sin(angle) * strengh;

        ShotTaken = true;

        if ( GunName == "Punch")
        {
            QListIterator<Player*> PlayerIter(*players);
            while (PlayerIter.hasNext())
            {
                Player *player = PlayerIter.next();
                QList<Worm*> *worms = player->returnWormList();
                if (worms->isEmpty() == false)
                {
                    QListIterator<Worm*> WormIter(*worms);
                    while (WormIter.hasNext())
                    {
                        Worm *worm = WormIter.next();
                        if (!(ActiveWorm == worm) && !(ActivePlayer == player))
                        {
                            if (abs(ActiveWorm->x()-worm->x()) < 40)
                            {
                                worm->ySpeed = projectileySpeed*0.4;
                                worm->xSpeed = projectilexSpeed*0.5;
                                worm->onGround = false;
                                worm->Health = worm->Health - 30;
                                worm->DisplayHealth();
                                return;
                            }
                        }
                    }
                }
            }
        }
        else if ( GunName == "Pistol" )
        {
            Bullet *bullet = new Bullet(this);
            bullet->setPistol();
            bullet->setxSpeed(cos(angle)*20);
            bullet->setySpeed(sin(angle)*20);
            bullets->append(bullet);
            changBulletAngle(bullet);

            if((Positionx - (ActiveWorm->x()+ActiveWorm->width())) > 10)
                bullet->move(ActiveWorm->x()+ActiveWorm->width() + 5
                             , ActiveWorm->y()+ActiveWorm->height()/2-bullet->height()/2);
            else if ((Positionx - (ActiveWorm->x())) < - 10)
                bullet->move(ActiveWorm->x() - 5 - bullet->width()
                             , ActiveWorm->y()+ActiveWorm->height()/2-bullet->height()/2);
            else if (((Positionx - (ActiveWorm->x())) > -10)&&((Positionx - (ActiveWorm->x()+ActiveWorm->width())) < 10))
                bullet->move(ActiveWorm->x() + ActiveWorm->width()/2 - bullet->width()/2
                             , ActiveWorm->y()-5 - bullet->height());
        }
        else if ( GunName == "RocketLauncher")
        {
            Bullet *bullet = new Bullet(this);
            bullet->setRocketLauncher();
            bullet->setxSpeed(projectilexSpeed*bullet->getmassEffect());
            bullet->setySpeed(projectileySpeed*bullet->getmassEffect());
            bullet->setRocketLauncher();
            bullets->append(bullet);
            changBulletAngle(bullet);

            if((Positionx - (ActiveWorm->x()+ActiveWorm->width())) > 10)
                bullet->move(ActiveWorm->x()+ActiveWorm->width() + 5
                             , ActiveWorm->y()+ActiveWorm->height()/2-bullet->height()/2);
            else if ((Positionx - (ActiveWorm->x())) < - 10)
                bullet->move(ActiveWorm->x() - 5 - bullet->width()
                             , ActiveWorm->y()+ActiveWorm->height()/2-bullet->height()/2);
            else if (((Positionx - (ActiveWorm->x())) > -10)&&((Positionx - (ActiveWorm->x()+ActiveWorm->width())) < 10))
                bullet->move(ActiveWorm->x() + ActiveWorm->width()/2 - bullet->width()/2
                             , ActiveWorm->y()-5 - bullet->height());
        }
        else if ( GunName == "Airstrike")
        {
            for (int i = 0 ; i < 3 ; i++)
            {
                Bullet *bullet = new Bullet(this);
                bullet->setAirstrike();
                bullet->setySpeed(4);
                bullet->move(Positionx + 150*i,-400*i);
                bullets->append(bullet);
                changBulletAngle(bullet);

            }
        }        
    }
}

void MainClass::WormDied(Player *p, QList<Worm *> *ws, Worm *w)
{
    deletedPlayersWorm = players->indexOf(p);

    if (SinglePlayer == true)
    {
        for (int i = 0 ; i < TurnAmounts ; i++)
        {
            if (TurnSuccession[i][0] == deletedPlayersWorm && TurnSuccession[i][1] == ws->size()-1)
            {
                TurnSuccession.removeAt(i);
                TurnAmounts --;
                break;
            }
        }
    }

    if (Multiplayer == true)
    {
        Datagram = "#Dead" + QByteArray::number(deletedPlayersWorm) + ":" + QByteArray::number(ws->indexOf(w));
        socket->writeDatagram(Datagram,EnemyIP,2356);
        WaitingAck->start();
    }
    if (w == ActiveWorm)
        emit EndmyTurn();

    w->deleteLater();
    w->HealthDisplay->deleteLater();
    w->NameDisplay->deleteLater();
    ws->removeAt(ws->indexOf(w));

    if (ws->size() == 0)
    {
        players->removeAt(players->indexOf(p));
        if (Multiplayer == true)
        {
            Player *player = players->at(0);
            Datagram = "#Done" + player->getName().toUtf8();
        }
    }
}

void MainClass::WormDied(int IndexPlayer, int IndexWorm)
{
    Player *player  = players->at(IndexPlayer);
    QList<Worm*> *worms = player->returnWormList();
    Worm *worm = worms->at(IndexWorm);
    worm->deleteLater();
    worm->HealthDisplay->deleteLater();
    worm->NameDisplay->deleteLater();
    worms->removeAt(IndexWorm);

    if (worms->size() == 0)
        players->removeAt(IndexPlayer);
}

void MainClass::changBulletAngle(Bullet *b)
{
    QPixmap Texture(b->getFileName());
    double RotationRadians = atan2(abs(b->getxSpeed()),abs(b->getySpeed()));

    if ( b->getySpeed() < 0 && b->getxSpeed() > 0)
        RotationRadians = RotationRadians - M_PI/2 ;
    else if ( b->getySpeed() < 0 && b->getxSpeed() < 0)
        RotationRadians = M_PI/2 -(RotationRadians - M_PI);
    else if ( b->getySpeed() > 0 && b->getxSpeed() < 0)
        RotationRadians = RotationRadians + M_PI/2;
    else if ( b->getySpeed() > 0 && b->getxSpeed() > 0)
        RotationRadians = M_PI/2 - RotationRadians;

    Texture = Texture.transformed(QTransform().rotateRadians(RotationRadians));
    b->setPixmap(Texture);
    b->resize(Texture.width(),Texture.height());
}

void MainClass::MapLabelClicked(QString filename, QString name)
{
    MapFileName = filename;
    MapName = name;

    QListIterator<MapLabel*> Iter(maps);
    while (Iter.hasNext())
    {
        MapLabel *map = Iter.next();
        map->deleteLater();
    }
    maps.clear();

    this->clearFocus();
    if (SinglePlayer == true)
    {
        StartGame();
        QRect position = frameGeometry();
        position.moveCenter(QDesktopWidget().availableGeometry().center());
        move(position.topLeft());
    }
    else if (Multiplayer == true)
    {
        ui->LobbyPage->show();
        ui->LHostname->setText(MyName);
        ui->BStartGame->setEnabled(true);
        resize(300,300);
        ui->LobbyPage->move(this->width()/2 - ui->LobbyPage->width()/2
                            , this->height()/2 - ui->LobbyPage->height()/2);
    }

}

void MainClass::on_BSinglePlayer_clicked()
{
    SinglePlayer = true;
    ui->InfoPage->show();
    ui->InfoPage->move(this->width()/2 - ui->InfoPage->width()/2,this->height()/2-ui->InfoPage->height()/2);
    ui->HomePage->hide();
}

void MainClass::on_BNext_clicked()
{
    AmountofPlayers = ui->SBPlayers->value();
    AmountofWorms = ui->SBWorms->value();
    ui->InfoPage->hide();
    int i = 0;
    QDirIterator iterator(Dir + "Maps//");
    iterator.next();
    iterator.next();
    while (iterator.hasNext())
    {
        QString fntemp = iterator.next();
        MapLabel *map = new MapLabel (this);
        QPixmap pic(fntemp);
        map->setPixmap(pic);
        map->setScaledContents(true);
        map->setMinimumSize(200,100);
        map->setMaximumSize(200,100);
        map->move(20 , 20 + 100 * i +20 *i);
        map->setFileName(fntemp);
        map->setName(iterator.fileName());
        map->show();
        maps.append(map);
        connect(map,SIGNAL(clicked(QString,QString)),this,SLOT(MapLabelClicked(QString,QString)));
        i ++;
    }
    resize(240 , 40 + 100 * i + 20 *i);
}

void MainClass::on_BMUltiPlayer_clicked()
{
    Multiplayer = true;
    AmountofWorms = 2;
    AmountofPlayers = 2;

    WaitingPlayer = new QTimer(this);
    WaitingPlayer->setInterval(1000);
    connect(WaitingPlayer,SIGNAL(timeout()),this,SLOT(WaitingPlayersFired()));

    WaitingAck = new QTimer(this);
    WaitingAck->setInterval(1000);
    connect(WaitingAck,SIGNAL(timeout()),this,SLOT(WaitingAckFired()));

    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    socket->bind(QHostAddress::Any,2356);

    ui->HomePage->hide();
    ui->MultiPlayerPage->show();
    ui->MultiPlayerPage->move(this->width()/2 - ui->MultiPlayerPage->width()/2,this->height()/2 - ui->MultiPlayerPage->height()/2);

    Datagram = MyName.toUtf8();
    socket->writeDatagram(Datagram,QHostAddress::Broadcast,2356);
}

void MainClass::on_BHost_clicked()
{
    Host = true;
    ui->MultiPlayerPage->hide();
    int i = 0;
    QDirIterator iterator(Dir + "Maps//");
    iterator.next();
    iterator.next();
    while (iterator.hasNext())
    {
        QString fntemp = iterator.next();
        MapLabel *map = new MapLabel (this);
        QPixmap pic(fntemp);
        map->setPixmap(pic);
        map->setScaledContents(true);
        map->setMinimumSize(200,100);
        map->setMaximumSize(200,100);
        map->move(20 , 20 + 100 * i +20 *i);
        map->setFileName(fntemp);
        map->setName(iterator.fileName());
        map->show();
        maps.append(map);
        connect(map,SIGNAL(clicked(QString,QString)),this,SLOT(MapLabelClicked(QString,QString)));
        i ++;
    }
    resize(240 , 40 + 100 * i + 20 *i);
    Datagram = "#Host" + MyName.toUtf8();
    socket->writeDatagram(Datagram,QHostAddress::Broadcast,2356);
    WaitingPlayer->start();
}

void MainClass::receiveData()
{
    if (Multiplayer == true)
    {
        while (socket->hasPendingDatagrams())
        {
            QByteArray datagramReceived , datagramReceivedInfo ;
            QList<QByteArray> Info;
            Info.clear();
            datagramReceived.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            socket->readDatagram(datagramReceived.data(), datagramReceived.size(), &sender, &senderPort);
            if (datagramReceived == MyName.toUtf8())
            {
                MyIP = sender;
                return;
            }
            if (sender == MyIP)
                return;
            else
            {
                datagramReceivedInfo = datagramReceived.left(5);
                datagramReceived.remove(0,5);
                if (datagramReceivedInfo == "#Host" && Client == true)
                {
                    ui->LHostedGame->setText(datagramReceived + "'s Game");
                    EnemyName = datagramReceived;
                    EnemyIP = sender;
                }
                else if (datagramReceivedInfo == "#Join" && Host == true)
                {
                    ui->Lplayername->setText(datagramReceived);
                    EnemyName = datagramReceived;
                    EnemyIP = sender;
                    WaitingPlayer->stop();
                }
                else if (datagramReceivedInfo == "#MapD")
                {
                    Info = datagramReceived.split(':');
                    collisiondetection->MapDestuction(Info[0].toInt() ,Info[1].toInt() ,Info[2].toInt());

                    Datagram = "#ACKN";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#Worm")
                    UpdateWormPositions(datagramReceived);
                else if (datagramReceivedInfo == "#Dead")
                {
                    Info = datagramReceived.split(':');
                    emit WormDied(Info[0].toInt() ,Info[1].toInt());

                    Datagram = "#ACKN";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#Done")
                {
                    GeneralTimer->stop();
                    displayFireWorks->resize(Map.width(),Map.height());
                    displayFireWorks->setScaledContents(true);
                    displayFireWorks->setVisible(true);
                    displayFireWorks->raise();
                    FireWorks->start();

                    WinnerDisplay->setText("The winner is " + datagramReceived + " !");
                    WinnerDisplay->adjustSize();
                    WinnerDisplay->move(Map.width()/2 - WinnerDisplay->width()/2 ,
                                        Map.height()/2 - WinnerDisplay->height()/2);
                    WinnerDisplay->setVisible(true);
                    WinnerDisplay->raise();

                    Datagram = "#ACKN";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#Your")
                {
                    if (Host == true)
                    {
                        Player *playerA  = players->at(0);
                        playerA->setActive();
                        ActivePlayer = playerA;
                        QList<Worm*> *wormsA = playerA->returnWormList();
                        Worm *wormA;
                        if (datagramReceived.toInt()+1 > wormsA->size()-1)
                            wormA = wormsA->at(0);
                        else
                            wormA = wormsA->at(datagramReceived.toInt()+1);

                        wormA->Active = true;
                        ActiveWorm = wormA;
                        keyhandler->setEnabled(true);
                        keyhandler->setFocus();
                        emit UpdateActiveWorm(wormA);
                    }
                    else if (Client == true)
                    {
                        Player *playerA  = players->at(1);
                        playerA->setActive();
                        ActivePlayer = playerA;
                        QList<Worm*> *wormsA = playerA->returnWormList();
                        Worm *wormA;
                        if (datagramReceived.toInt() > wormsA->size()-1)
                            wormA = wormsA->at(0);
                        else
                            wormA = wormsA->at(datagramReceived.toInt());

                        wormA->Active = true;
                        ActiveWorm = wormA;
                        keyhandler->setEnabled(true);
                        keyhandler->setFocus();

                        emit UpdateActiveWorm(wormA);
                    }

                    QTimer::singleShot(1500,this,SLOT(MultiplayerTurnChange()));

                    Datagram = "#ACKY";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#GO!!")
                {
                    MapFileName = Dir + "Maps//" + datagramReceived;
                    StartGame();
                    QRect position = frameGeometry();
                    position.moveCenter(QDesktopWidget().availableGeometry().center());
                    move(position.topLeft());

                    keyhandler->setEnabled(false);
                    ui->LobbyPage->hide();
                    Datagram = "#ACKG";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#Name")
                {
                    UpdateWormNames(datagramReceived);
                    Datagram = "#ACK!";
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#ACKN")
                {
                    WaitingAck->stop();
                }
                else if (datagramReceivedInfo == "#ACKG")
                {
                    WaitingAck->stop();
                    Datagram = "#Name" + ConstructWormNamedatagram();
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                    WaitingAck->start();
                }
                else if (datagramReceivedInfo == "#ACKY")
                {
                    WaitingAck->stop();
                    Datagram = "#Worm" + ConstructWormPositiondatagram();
                    socket->writeDatagram(Datagram,EnemyIP,2356);
                }
                else if (datagramReceivedInfo == "#ACK!")
                {
                    WaitingAck->stop();
                    Datagram = "#Worm" + ConstructWormPositiondatagram();
                    socket->writeDatagram(Datagram,EnemyIP,2356);

                   QTimer::singleShot(1500,this,SLOT(MultiplayerTurnChange()));
                }
            }
        }
    }
}

void MainClass::WaitingPlayersFired()
{
    socket->writeDatagram(Datagram,QHostAddress::Broadcast,2356);
}

void MainClass::WaitingAckFired()
{
    // ACTIVATE IF NETWORK NOISE IS HIGH
  //  socket->writeDatagram(Datagram,EnemyIP,2356);
}

void MainClass::MultiplayerTurnChange()
{
    MyTurn = true;
    GeneralTimer->start();
}

void MainClass::on_BJoinGame_clicked()
{
    if (ui->LHostedGame->text() != "No games found")
    {
        ui->LobbyPage->show();
        ui->LobbyPage->move(this->width()/2 - ui->LobbyPage->width()/2
                            , this->height()/2 - ui->LobbyPage->height()/2);
        ui->JoinPage->hide();
        ui->LHostname->setText(EnemyName);
        ui->Lplayername->setText(MyName);

        Datagram = "#Join" + MyName.toUtf8();
        socket->writeDatagram(Datagram,EnemyIP,2356);
    }
}

void MainClass::on_BStartGame_clicked()
{
    if (ui->Lplayername->text() != "Waiting for player")
    {
        StartGame();
        QRect position = frameGeometry();
        position.moveCenter(QDesktopWidget().availableGeometry().center());
        move(position.topLeft());
        ui->LobbyPage->hide();
        Datagram = "#GO!!" + MapName.toUtf8();
        socket->writeDatagram(Datagram,EnemyIP,2356);
        WaitingAck->start();
    }
}

void MainClass::on_BJoin_clicked()
{
    ui->JoinPage->show();
    Client = true;
    ui->MultiPlayerPage->hide();
    ui->JoinPage->move(this->width()/2 - ui->JoinPage->width()/2 , this->height()/2 - ui->JoinPage->height()/2);
}
