#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "arrow.h"
#include "bomb.h"
#include "cupola.h"
#include "arrowtower.h"
#include "audioplayer.h"
#include "plistreader.h"
#include "towerposition.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>

static const int TowerCost = 300;
static const int UpgradeCost = 300;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_waves(0)
    , m_playerHp(50)
	, m_playrGold(1000)
	, m_gameEnded(false)
	, m_gameWin(false)
{
	ui->setupUi(this);

	preLoadWavesInfo();
	loadTowerPositions();
	addWayPoints();

	m_audioPlayer = new AudioPlayer(this);
	m_audioPlayer->startBGM();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(15);

	// 设置300ms后游戏启动
	QTimer::singleShot(300, this, SLOT(gameStart()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadTowerPositions()
{
	QFile file(":/config/TowersPosition.plist");
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
		return;
	}

	PListReader reader;
	reader.read(&file);

	QList<QVariant> array = reader.data();
	foreach (QVariant dict, array)
	{
		QMap<QString, QVariant> point = dict.toMap();
		int x = point.value("x").toInt();
		int y = point.value("y").toInt();
		m_towerPositionsList.push_back(QPoint(x, y));
	}

	file.close();
}

void MainWindow::paintEvent(QPaintEvent *)
{
	if (m_gameEnded || m_gameWin)
	{
		QString text = m_gameEnded ? "YOU LOST!!!" : "YOU WIN!!!";
		QPainter painter(this);
		painter.setPen(QPen(Qt::red));
		painter.drawText(rect(), Qt::AlignCenter, text);
		return;
	}

    QPixmap cachePix(":/image/map.png");
	QPainter cachePainter(&cachePix);

	foreach (const TowerPosition &towerPos, m_towerPositionsList)
		towerPos.draw(&cachePainter);

    foreach (const Cupola *cupola, m_cupolasList)
        cupola->draw(&cachePainter);

    foreach (const ArrowTower *arrowtower, m_arrowtowersList)
        arrowtower->draw(&cachePainter);

	foreach (const Enemy *enemy, m_enemyList)
		enemy->draw(&cachePainter);

    foreach (const Bomb *bomb, m_bombList)
        bomb->draw(&cachePainter);

    foreach (const Arrow *arrow, m_arrowList)
        arrow->draw(&cachePainter);

	drawWave(&cachePainter);
	drawHP(&cachePainter);
	drawPlayerGold(&cachePainter);

	QPainter painter(this);
	painter.drawPixmap(0, 0, cachePix);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	QPoint pressPos = event->pos();
    for ( int i=0; i!=m_towerPositionsList.size(); ++i )
    {
        if (canBuyTower() && !m_towerPositionsList.at(i).hasTower() && m_towerPositionsList.at(i).containPoint(pressPos))
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playrGold -= TowerCost;
            m_towerPositionsList.at(i).setHasTower();
            Cupola *cupola = new Cupola(m_towerPositionsList.at(i).centerPos(), this);
            m_cupolasList.push_back(cupola);
            update();
            break;
        }
        else if(canUpgradeTower() && m_towerPositionsList.at(i).hasTower() && m_towerPositionsList.at(i).containPoint(pressPos))
        {
            for(int j=0; j!=m_cupolasList.size(); ++j )
            {
                if(m_cupolasList.at(j)->m_pos == m_towerPositionsList.at(i).centerPos())
                {

                    m_audioPlayer->playSound(TowerPlaceSound);
                    m_playrGold -= UpgradeCost;
                    m_cupolasList.removeAt(j);
                    ArrowTower *arrowtower = new ArrowTower(m_towerPositionsList.at(i).centerPos(), this);
                    m_arrowtowersList.push_back(arrowtower);
                    update();
                    break;
                }
            }
        }
    }
}

bool MainWindow::canBuyTower() const
{
	if (m_playrGold >= TowerCost)
		return true;
	return false;
}

bool MainWindow::canUpgradeTower() const
{
    if (m_playrGold >= UpgradeCost)
        return true;
    return false;
}

void MainWindow::drawWave(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
	painter->setPen(QPen(Qt::red));
	painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
}

void MainWindow::doGameOver()
{
	if (!m_gameEnded)
	{
		m_gameEnded = true;
		// 此处应该切换场景到结束场景
		// 暂时以打印替代,见paintEvent处理
	}
}

void MainWindow::awardGold(int gold)
{
	m_playrGold += gold;
	update();
}

AudioPlayer *MainWindow::audioPlayer() const
{
	return m_audioPlayer;
}

void MainWindow::addWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(1100, 1050));
	m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(1100, 360));
	m_wayPointsList.push_back(wayPoint2);
	wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(930, 360));
	m_wayPointsList.push_back(wayPoint3);
	wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(930, 200));
	m_wayPointsList.push_back(wayPoint4);
	wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(615, 200));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(615, 740));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint7 = new WayPoint(QPoint(80, 740));
    m_wayPointsList.push_back(wayPoint7);
    wayPoint7->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint8 = new WayPoint(QPoint(80, 900));
    m_wayPointsList.push_back(wayPoint8);
    wayPoint8->setNextWayPoint(wayPoint7);

}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
	m_audioPlayer->playSound(LifeLoseSound);
	m_playerHp -= damage;
	if (m_playerHp <= 0)
		doGameOver();
}

void MainWindow::removedEnemy(Enemy *enemy)
{
	Q_ASSERT(enemy);

	m_enemyList.removeOne(enemy);
	delete enemy;

	if (m_enemyList.empty())
	{
		++m_waves;
		if (!loadWave())
		{
			m_gameWin = true;
			// 游戏胜利转到游戏胜利场景
			// 这里暂时以打印处理
		}
	}
}

void MainWindow::removedBomb(Bomb *bomb)
{
    Q_ASSERT(bomb);

    m_bombList.removeOne(bomb);
    delete bomb;
}

void MainWindow::addBomb(Bomb *bomb)
{
    Q_ASSERT(bomb);

    m_bombList.push_back(bomb);
}

void MainWindow::removedArrow(Arrow *arrow)
{
    Q_ASSERT(arrow);

    m_arrowList.removeOne(arrow);
    delete arrow;
}

void MainWindow::addArrow(Arrow *arrow)
{
    Q_ASSERT(arrow);

    m_arrowList.push_back(arrow);
}

void MainWindow::updateMap()
{
	foreach (Enemy *enemy, m_enemyList)
		enemy->move();
    foreach (Cupola *cupola, m_cupolasList)
        cupola->checkEnemyInRange();
    foreach (ArrowTower *arrowtower, m_arrowtowersList)
        arrowtower->checkEnemyInRange();
	update();
}

void MainWindow::preLoadWavesInfo()
{
	QFile file(":/config/Waves.plist");
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
		return;
	}

	PListReader reader;
	reader.read(&file);

	// 获取波数信息
	m_wavesInfo = reader.data();

	file.close();
}

bool MainWindow::loadWave()
{
	if (m_waves >= m_wavesInfo.size())
		return false;

	WayPoint *startWayPoint = m_wayPointsList.back();
	QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

	for (int i = 0; i < curWavesInfo.size(); ++i)
	{
		QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
		int spawnTime = dict.value("spawnTime").toInt();

		Enemy *enemy = new Enemy(startWayPoint, this);
		m_enemyList.push_back(enemy);
		QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
	}

	return true;
}

QList<Enemy *> MainWindow::enemyList() const
{
	return m_enemyList;
}

void MainWindow::gameStart()
{
	loadWave();
}
