#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class WayPoint;
class QPainter;
class MainWindow;
class Cupola;

class Enemy : public QObject
{
	Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap(":/image/enemyBug.png"));
	~Enemy();

	void draw(QPainter *painter) const;
	void move();
	void getDamage(int damage);
	void getRemoved();
    void getAttacked(Cupola *attacker);
    void gotLostSight(Cupola *attacker);
	QPoint pos() const;
    static int count;

public slots:
	void doActivate();

private:
	bool			m_active;
	int				m_maxHp;
	int				m_currentHp;
	qreal			m_walkingSpeed;
	qreal			m_rotationSprite;

	QPoint			m_pos;
	WayPoint *		m_destinationWayPoint;
	MainWindow *	m_game;
    QList<Cupola *>	m_attackedTowersList;

	const QPixmap	m_sprite;
	static const QSize ms_fixedSize;
};

#endif // ENEMY_H
