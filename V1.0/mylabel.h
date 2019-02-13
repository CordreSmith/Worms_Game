#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>
#include <QImage>

class myLabel : public QLabel
{
    Q_OBJECT
public:
    myLabel(QWidget *parent = 0 );
    void setName(QString N);
    void setTexture();

signals:
    void clicked(QString name);

protected:
    void mousePressEvent( QMouseEvent *event);

private:
    QString Name , FileName;
    QImage GunTexture;

};

#endif // MYLABEL_H
