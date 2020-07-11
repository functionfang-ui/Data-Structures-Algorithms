#include "arrowtower.h"
#include "enemy.h"
#include "arrow.h"
#include "audioplayer.h"
#include "mainwindow.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>

const QSize ArrowTower::ms_arrowTower_fixedSize(70, 107);

ArrowTower::ArrowTower(QPoint pos, MainWindow *game, const QPixmap &sprite)
    : Cupola(pos, game, sprite)
{
    upgrade();
}

ArrowTower::~ArrowTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void ArrowTower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    // 绘制攻击范围
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_arrowTower_fixedSize.width() / 2, -ms_arrowTower_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void ArrowTower::shootWeapon()
{
    Arrow *arrow = new Arrow(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
    arrow->move();
    m_game->audioPlayer()->playSound(ArrowShootSound);
    m_game->addArrow(arrow);
}

void ArrowTower::upgrade()
{
    m_fireRate = 300;
    m_attackRange = 400;
    m_damage = 8;
}
