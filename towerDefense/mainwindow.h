#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "towerposition.h"
#include "cupola.h"
#include "arrowtower.h"

namespace Ui {
class MainWindow;
}

class WayPoint;
class Enemy;
class Bomb;
class Arrow;
class AudioPlayer;
class TowerPosition;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void getHpDamage(int damage = 1);
	void removedEnemy(Enemy *enemy);
    void removedBomb(Bomb *bomb);
    void addBomb(Bomb *bomb);
    void removedArrow(Arrow *arrow);
    void addArrow(Arrow *arrow);
	void awardGold(int gold);

	AudioPlayer* audioPlayer() const;
	QList<Enemy *> enemyList() const;

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);

private slots:
	void updateMap();
	void gameStart();

private:
	void loadTowerPositions();
	void addWayPoints();
	bool loadWave();
	bool canBuyTower() const;
    bool canUpgradeTower() const;
	void drawWave(QPainter *painter);
	void drawHP(QPainter *painter);
	void drawPlayerGold(QPainter *painter);
	void doGameOver();
	void preLoadWavesInfo();

private:
	Ui::MainWindow *		ui;
	int						m_waves;
	int						m_playerHp;
	int						m_playrGold;
	bool					m_gameEnded;
	bool					m_gameWin;
	AudioPlayer *			m_audioPlayer;
	QList<QVariant>			m_wavesInfo;
	QList<TowerPosition>	m_towerPositionsList;
    QList<Cupola *>			m_cupolasList;
    QList<ArrowTower *>	    m_arrowtowersList;
	QList<WayPoint *>		m_wayPointsList;
	QList<Enemy *>			m_enemyList;
    QList<Bomb *>			m_bombList;
    QList<Arrow *>          m_arrowList;
};

#endif // MAINWINDOW_H
