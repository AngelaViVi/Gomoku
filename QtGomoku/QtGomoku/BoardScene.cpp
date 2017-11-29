#include "BoardScene.h"
#include <QGraphicsSceneMouseEvent>
#include <qDebug>

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
	qDebug() << "X:" << event->scenePos().x() << "Y:" << event->scenePos().y()<<"RX:" << event->scenePos().rx() << "RY:" << event->scenePos().ry();
	//qDebug() << "X:" << event->screenPos().x() << "Y:" << event->screenPos().y()<<"RX:" << event->screenPos().rx() << "RY:" << event->screenPos().ry();
}
