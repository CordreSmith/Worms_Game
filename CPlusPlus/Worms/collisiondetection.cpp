#include "collisiondetection.h"

CollisionDetection::CollisionDetection(QWidget *parent)
{
    setParent(parent);
}

int CollisionDetection::movementCollision(Worm *w)
{
    if (w->Left == true)
    {
        int BL_x = w->x();
        int BL_y = w->y() + w->height();
        int ML_x = w->x();
        int ML_y = w->y() + round(w->width()/2);

        if (ArrMap[BL_x-1][BL_y-1] == 1 && ArrMap[ML_x-1][ML_y-1] == 1 && BL_x-1 > 0 )
            return (0);
        else if (ArrMap[BL_x-1][BL_y-1] == 0 && ArrMap[ML_x-1][ML_y-1] == 1 && BL_x-1 > 0 )
            return (0);
        else if (BL_x-2 <= 0)
            return (0);
        else if (BL_y+2 >= parentWidget()->height())
            return (0);
        else if (ArrMap[BL_x-1][BL_y-1] == 1 && ArrMap[ML_x-1][ML_y-1] == 0 && BL_x-1 > 0 )
            return (1);
        else if (ArrMap[BL_x-1][BL_y-1] == 0 && ArrMap[ML_x-1][ML_y-1] == 0 && BL_x-1 > 0 )
            return (2);
    }
    else if (w->Right == true)
    {
        int BR_x = w->x() + w->width();
        int BR_y = w->y() + w->height();
        int MR_x = w->x() + w->width();
        int MR_y = w->y() + round(w->width()/2);

        if (ArrMap[BR_x+1][BR_y+1] == 1 && ArrMap[MR_x+1][MR_y+1] == 1)
            return (0);
        else if (ArrMap[BR_x+1][BR_y+1] == 0 && ArrMap[MR_x+1][MR_y+1] == 1)
            return (0);
        else if (BR_x+2 >= parentWidget()->width())
            return (0);
        else if (BR_y+2 >= parentWidget()->height())
            return (0);
        else if (ArrMap[BR_x+1][BR_y+1] == 1 && ArrMap[MR_x+1][MR_y+1] == 0)
            return (1);
        else if (ArrMap[BR_x+1][BR_y+1] == 0 && ArrMap[MR_x+1][MR_y+1] == 0)
            return (2);
    }
    else
        return (0);

    return(0);
}

bool CollisionDetection::onMap(Worm *w)
{
    if (w->x() - w->xSpeed <= 0 || w->x()+w->width() + w->xSpeed >= parentWidget()->width() )
        return false;

    else if (w->y() - w->ySpeed <= 0 || w->y() + w->height() + w->ySpeed >= parentWidget()->height())
        return false;

    return true;
}

bool CollisionDetection::bulletWormCollision(Bullet *b)
{
    if (b->getWormHit() == true)
    {
        QListIterator<Player*> PlayerIter(*players);
        while(PlayerIter.hasNext())
        {
            Player *player = PlayerIter.next();
            QList<Worm*> *worms = player->returnWormList();

            QListIterator<Worm*> WormIter(*worms);
            while(WormIter.hasNext())
            {
                Worm *worm = WormIter.next();

                for (int y = 0 ; y < worm->height() ; y++)
                {
                    for (int x = 0 ; x < worm->width() ; x++)
                    {
                        if ((b->x()+ round(b->width()/2) == worm->x()+x)&&(b->y()+(b->height()/2) == worm->y()+y))
                        {
                            worm->Health -= b->getDamage();
                            if (worm->Health <= 0)
                            {
                                worm->DisplayHealth();
                                emit WormOutofBounds(player,worms,worm);
                            }
                            else worm->DisplayHealth();

                            MapDestuction(b);
                            return true;
                        }
                    }
                }
            }
        }
    }


    return false;
}

bool CollisionDetection::bulletTerrainCollision(Bullet *b)
{
    if ((b->x()+b->getxSpeed()+b->width() <= parentWidget()->width())
            && (b->y()+b->getySpeed()+b->height() <= parentWidget()->height())
            && (b->x()+b->getxSpeed() >= 0)
            && (b->y()+b->getySpeed() >= 0))
    {
        if (ArrMap[b->x()+round(b->width()/2)][b->y()+round(b->height()/2)] == 1)
        {
            if (b->getBounce() == false)
                MapDestuction(b);
                DamageCalculator(b);
            return true;
        }
    }

    return false;
}

void CollisionDetection::MapDestuction(Bullet *b)
{
    double Radius , calculatedRadius ;

    Radius = b->getAoE();

    int CenterxPoint , CenteryPoint;

    CenterxPoint = b->x()+(b->width()/2);
    CenteryPoint = b->y()+(b->height()/2);

    for (int x = CenterxPoint-Radius ; x <= CenterxPoint + Radius ; x++)
    {
        for (int y = CenteryPoint - Radius ; y <= CenteryPoint + Radius ; y++)
        {
            calculatedRadius = sqrt((CenterxPoint-x)*(CenterxPoint-x) + (CenteryPoint-y)*(CenteryPoint-y)) ;
            if ( calculatedRadius <= Radius)
            {
                if (((CenterxPoint+abs(CenterxPoint-x)) < parentWidget()->width())
                   && (CenteryPoint+abs(CenteryPoint-y) < parentWidget()->height())
                   && (CenterxPoint-abs(CenterxPoint-x) >= 0)
                   && (CenteryPoint-abs(CenteryPoint-y) >= 0))
                {
                    ArrMap[x][y] = 0;
                    Map.setPixel(x,y,Qt::white);

                }
            }
        }
    }

    emit updateMap(Map);

}

void CollisionDetection::MapDestuction(int xpoint, int ypoint, int Radius)
{
    double calculatedRadius;

    for (int x = xpoint-Radius ; x <= xpoint + Radius ; x++)
    {
        for (int y = ypoint - Radius ; y <= ypoint + Radius ; y++)
        {
            calculatedRadius = sqrt((xpoint-x)*(xpoint-x) + (ypoint-y)*(ypoint-y)) ;
            if ( calculatedRadius <= Radius)
            {
                ArrMap[x][y] = 0;
                Map.setPixel(x,y,Qt::white);

            }
        }
    }

    emit updateMap(Map);
}

void CollisionDetection::DamageCalculator(Bullet *b)
{
    QListIterator<Player*> PlayerIter(*players);
    while(PlayerIter.hasNext())
    {
        Player *player = PlayerIter.next();
        QList<Worm*> *worms = player->returnWormList();
        QListIterator<Worm*> WormIter(*worms);
        while(WormIter.hasNext())
        {
            Worm *worm = WormIter.next();
            double wormcenterx = worm->x() + worm->width()/2;
            double wormcentery = worm->y() + worm->height()/2;
            double bulletcenterx = b->x() + b->width()/2;
            double bulletcentery = b->y() + b->height()/2;
            double DistanceWormBullet = sqrt((wormcenterx - bulletcenterx)*(wormcenterx - bulletcenterx)
                                             + (wormcentery - bulletcentery)*(wormcentery - bulletcentery));
            if (DistanceWormBullet <= b->getAoE())
                worm->Health -= b->getDamage();
        }
    }
}

void CollisionDetection::setValues(QVector<QVector<int> > AM, QList<Player *> *p, QImage m)
{
    ArrMap = AM;
    players = p;
    Map = m;
}

void CollisionDetection::freefallDetection(Worm *w)
{
    if (w->onGround == true)
    {
        bool Temp = false;
        if (w->Direction == false)
        {
            for (int i = 0 ; i <= w->width()/2;i++)
            {
                if (ArrMap[w->x()+i][w->y()+w->height()+1] == 1)
                {
                    Temp = true;
                    break;
                }
            }
        }
        else if (w->Direction == true)
        {
            for (int i = w->width()/2 ; i <= w->width()-1;i++)
            {
                if (ArrMap[w->x()+i][w->y()+w->height()+1] == 1)
                {
                    Temp = true;
                    break;
                }
            }
        }


        if (Temp == false)
        {
            w->onGround = false;
            w->ySpeed = 1;
        }
    }
}

void CollisionDetection::freefallCollision(Worm *w)
{
    if ((w->onGround == false)&&(w->ySpeed>0))
    {
        if (w->Direction == false)
        {
            for (int i = 0 ; i <= w->width()/2;i++)
            {
                if (ArrMap[w->x()+i][w->y()+w->height()] == 1)
                {
                    w->onGround = true;
                    if (w->ySpeed >= 5)
                    {
                        double Damage = (round(w->ySpeed) - 5 ) * 14.2;
                        w->Health -= Damage;
                        w->DisplayHealth();
                    }
                    w->ySpeed = 0;
                    w->xSpeed = 0;
                    break;
                }
            }
        }
        else if (w->Direction == true)
        {
            for (int i = w->width()/2 ; i <= w->width()-1;i++)
            {
                if (ArrMap[w->x()+i][w->y()+w->height()] == 1)
                {
                    w->onGround = true;
                    if (w->ySpeed >= 5)
                    {
                        double Damage = (w->ySpeed - 5 ) * 14.2;
                        w->Health -= Damage;
                        w->DisplayHealth();
                    }
                    w->ySpeed = 0;
                    w->xSpeed = 0;
                    break;
                }
            }
        }
    }
}
