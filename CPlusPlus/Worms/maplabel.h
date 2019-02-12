#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QLabel>
#include <QMouseEvent>


class MapLabel : public QLabel
{
    Q_OBJECT
public:
    MapLabel(QWidget *parent = 0);

    void setFileName(QString FN);
    void setName(QString N);

private:
    QString FileName , Name;

    void mousePressEvent( QMouseEvent *event);

signals:
    void clicked(QString filename , QString name);
};

#endif // MAPLABEL_H
