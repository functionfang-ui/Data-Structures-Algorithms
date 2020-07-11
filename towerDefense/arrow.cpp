#include "arrow.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QtMath>
#include <QPainter>
#include <QVector2D>
#include <QPropertyAnimation>

const QSize Arrow::ms_arrow_fixedSize(50,10);

Arrow::Arrow(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
               MainWindow *game, const QPixmap &sprite)
    : m_startPos(startPos)
    , m_targetPos(targetPoint)
    , m_sprite(sprite)
    , m_currentPos(startPos)
    , m_target(target)
    , m_game(game)
    , m_damage(damage)
{
}
//这里需要大改！！！
void Arrow::draw(QPainter *painter) const
{
    QVector2D normalized(m_targetPos - m_startPos);
    normalized.normalize();
    qreal rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x()));
    painter->save();
    painter->translate(m_currentPos);
    painter->rotate(rotationSprite);
    static const QPoint offsetPoint(-ms_arrow_fixedSize.width() / 2, -ms_arrow_fixedSize.height() / 2);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Arrow::move()
{
    // 300毫秒内击中敌人
    static const int duration = 200;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(m_startPos);
    animation->setEndValue(m_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));
    animation->start();

}

void Arrow::hitTarget()
{
    // 这样处理的原因是:
    // 可能支箭矢击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续箭矢再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->enemyList().indexOf(m_target) != -1)
        m_target->getDamage(m_damage);
    m_game->removedArrow(this);
}

void Arrow::setCurrentPos(QPoint pos)
{
    m_currentPos = pos;
}

QPoint Arrow::currentPos() const
{
    return m_currentPos;
}
