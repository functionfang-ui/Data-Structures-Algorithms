#include "bomb.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QPainter>
#include <QtMath>
#include <QVector2D>
#include <QPropertyAnimation>

const QSize Bomb::ms_bomb_fixedSize(8, 8);

Bomb::Bomb(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
			   MainWindow *game, const QPixmap &sprite/* = QPixmap(":/image/bullet.png")*/)
	: m_startPos(startPos)
	, m_targetPos(targetPoint)
	, m_sprite(sprite)
	, m_currentPos(startPos)
	, m_target(target)
	, m_game(game)
	, m_damage(damage)
{}

void Bomb::draw(QPainter *painter) const
{
    QVector2D normalized(m_targetPos - m_startPos);
    normalized.normalize();
    qreal rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x()));
    painter->save();
    painter->translate(m_currentPos);
    painter->rotate(rotationSprite);
    static const QPoint offsetPoint(-ms_bomb_fixedSize.width() / 2, -ms_bomb_fixedSize.height() / 2);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Bomb::move()
{
    // 100毫秒内击中敌人
    static const int duration = 300;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(m_startPos);
    animation->setEndValue(m_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));
    animation->start();

}

void Bomb::hitTarget()
{
	// 这样处理的原因是:
	// 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
	// 后续炮弹再攻击到的敌人就是无效内存区域
	// 因此先判断下敌人是否还有效
	if (m_game->enemyList().indexOf(m_target) != -1)
		m_target->getDamage(m_damage);
    m_game->removedBomb(this);
}

void Bomb::setCurrentPos(QPoint pos)
{
	m_currentPos = pos;
}

QPoint Bomb::currentPos() const
{
	return m_currentPos;
}
