#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "Alpha-Beta.h"


class QtGomoku : public QMainWindow
{
	Q_OBJECT

public:
	QtGomoku(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGomokuClass ui;
	BoardScene * m_scene;
	QPixmap * WhiteStone;
	QPixmap * BlackStone;
	AiAgent *m_ai;
public:
	Color HumanColor = black;	//����ִ������ɫ����
	Color AiColor = white;		//Aiִ������ɫ����
	Player CurrentTurn = Human; //��ǰ�ֵ�˭����
	Color BoardMap[15][15];		//�������

private:
	void UpdateWholeBoardView();//���ݵ�ǰ�������ˢ����������
	void PutDownStone(int x,int y);//����
public slots:
	void OnMouseReleaseOnBoardSense(QPoint *pos);//����������Ͽ��ͷ�
};
