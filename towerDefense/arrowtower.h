#ifndef ARROWTOWER_H
#define ARROWTOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include "cupola.h"

class QPainter;
class Enemy;
class MainWindow;
class QTimer;
class Cupola;

class ArrowTower : public Cupola
{

public:
    ArrowTower(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap(":/image/arrowTower.png"));
    ~ArrowTower();

    void draw(QPainter *painter) const;
    void upgrade();

private slots:
    void shootWeapon();

    static const QSize ms_arrowTower_fixedSize;
};


#endif // ARROWTOWER_H
