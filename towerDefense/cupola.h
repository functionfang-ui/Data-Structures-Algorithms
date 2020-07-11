#ifndef CUPOLA_H
#define CUPOLA_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;
class QTimer;

class Cupola : QObject
{
	Q_OBJECT
public:
    Cupola(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap(":/image/cupola.png"));
    ~Cupola();

	void draw(QPainter *painter) const;
	void checkEnemyInRange();
	void targetKilled();
    void attackEnemy();
	void chooseEnemyForAttack(Enemy *enemy);
	void lostSightOfEnemy();

private slots:
    virtual void shootWeapon();

public:
	bool			m_attacking;
	int				m_attackRange;	// 代表塔可以攻击到敌人的距离
	int				m_damage;		// 代表攻击敌人时造成的伤害
	int				m_fireRate;		// 代表再次攻击敌人的时间间隔
	qreal			m_rotationSprite;

	Enemy *			m_chooseEnemy;
	MainWindow *	m_game;
	QTimer *		m_fireRateTimer;

	const QPoint	m_pos;
    const QPixmap	m_sprite;

    static const QSize ms_cupola_fixedSize;
};

#endif // CUPOLA_H
