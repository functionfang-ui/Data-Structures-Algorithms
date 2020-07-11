#include "cupola.h"
#include "enemy.h"
#include "bomb.h"
#include "audioplayer.h"
#include "mainwindow.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>

const QSize Cupola::ms_cupola_fixedSize(42, 42);

Cupola::Cupola(QPoint pos, MainWindow *game, const QPixmap &sprite/* = QPixmap(":/image/tower.png"*/)
	: m_attacking(false)
    , m_attackRange(200)
    , m_damage(8)
    , m_fireRate(400)
	, m_rotationSprite(0.0)
	, m_chooseEnemy(NULL)
	, m_game(game)
	, m_pos(pos)
	, m_sprite(sprite)
{
	m_fireRateTimer = new QTimer(this);
	connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}

Cupola::~Cupola()
{
	delete m_fireRateTimer;
	m_fireRateTimer = NULL;
}

void Cupola::checkEnemyInRange()
{
	if (m_chooseEnemy)
	{
		// 这种情况下,需要旋转炮台对准敌人
		// 向量标准化
		QVector2D normalized(m_chooseEnemy->pos() - m_pos);
		normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x()));

		// 如果敌人脱离攻击范围
        if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
			lostSightOfEnemy();
	}
	else
	{
		// 遍历敌人,看是否有敌人在攻击范围内
		QList<Enemy *> enemyList = m_game->enemyList();
		foreach (Enemy *enemy, enemyList)
		{
			if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
			{
				chooseEnemyForAttack(enemy);
				break;
			}
		}
	}
}

void Cupola::draw(QPainter *painter) const
{
	painter->save();
	painter->setPen(Qt::white);
	// 绘制攻击范围
	painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

	// 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_cupola_fixedSize.width() / 2, -ms_cupola_fixedSize.height() / 2);
	// 绘制炮塔并选择炮塔
	painter->translate(m_pos);
	painter->rotate(m_rotationSprite);
	painter->drawPixmap(offsetPoint, m_sprite);
	painter->restore();
}

void Cupola::attackEnemy()
{
	m_fireRateTimer->start(m_fireRate);
}

void Cupola::chooseEnemyForAttack(Enemy *enemy)
{
	m_chooseEnemy = enemy;
	attackEnemy();
	m_chooseEnemy->getAttacked(this);
}
//bullet<->arrow
void Cupola::shootWeapon()
{
    Bomb *bomb = new Bomb(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
    bomb->move();
    m_game->audioPlayer()->playSound(LaserShootSound);
    m_game->addBomb(bomb);
}

void Cupola::targetKilled()
{
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
	m_rotationSprite = 0.0;
}

void Cupola::lostSightOfEnemy()
{
	m_chooseEnemy->gotLostSight(this);
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
	m_rotationSprite = 0.0;
}
