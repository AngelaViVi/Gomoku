#include "BoardScene.h"
#include <QGraphicsSceneMouseEvent>
#include <qDebug>
#include <QGraphicsScene>


BoardScene::BoardScene(QObject *parent)
	: QGraphicsScene(parent)
{
}

BoardScene::~BoardScene()
{
}
/*
 * ��������ƶ��¼�
 */
void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	event->pos().x();
	//qDebug() << "X:"<<event->pos().x()<<"Y:"<< event->pos().y();
}
/*
 * ��갴��
 */
void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
}
/*
 * ����ͷ�
 */
void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//qDebug() << "X:" << event->scenePos().x() << "Y:" << event->scenePos().y();
	emit BoardSenseMouseRelease(&QPoint(event->scenePos().x(), event->scenePos().y()));
}
