#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "AlphaBetaRobot.h"

/*
 *
 */
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
	QPixmap * MarkForLastStep;
	QGraphicsItem * Mark;
	QGraphicsTextItem *txtitem;
	//AiAgent *m_ai;
	AlphaBetaRobot * robot;
	int Count;					   //�������µ������ǵڼ���
	bool isPVE = true;			   //��Ϸģʽ�Ƿ�Ϊ�˻�
	bool isHumanGetBlack = true;   //����Ƿ�ѡ��ִ������
	bool isEnableForbidden = false;//�Ƿ���������
	bool m_isGameOver = true;	   //��Ϸ�Ƿ����
public:
	Color HumanColor = black;	//����ִ������ɫ����
	Color AiColor = white;		//Aiִ������ɫ����
	Player CurrentTurn = Human; //�˻�ģʽ�µ�ǰ�ֵ�˭����
	Color CurrentColor = black; //����ģʽ�µ�ǰ�ֵ�������ɫ����
	Color BoardMap[15][15];		//�������,�������ֻ������������ӵ�ʱ���ж��Ƿ�Ϸ�,���ж���Ϸ�Ƿ����

private:
	void UpdateWholeBoardView();//���ݵ�ǰ�������ˢ����������
	void PutDownStone(int x,int y);//����
	bool isGameOver();//�ж���Ϸ�Ƿ����
signals:
	void IsTimeForAiToCalculate(POINT*);//����ʱ����������ӵ�
public slots:
	void OnMouseReleaseOnBoardSense(QPoint *pos);//����������Ͽ��ͷ�
	void OnAiComplete(POINT * pos);				 //��ӦAi�̼߳������
	void OnNewGame();							 //����:����Ϸ
	void OnUndo();								 //����:����
	void OnStopGame();							 //����:������Ϸ
	void OnHelp();								 //����:����
	void OnCheckedHumanBlack();					 //��ѡ:���ִ��
	void OnCheckedHumanWhite();				     //��ѡ:���ִ��
	void OnCheckedEnableForbidden();			 //��ѡ:��������
	void OnCheckedUnableForbidden();			 //��ѡ:�رս���
	void OnCheckedPVE();						 //��ѡ:�˻���ս
	void OnCheckedPVP();						 //��ѡ:���˶�ս
	void OnAiLevelChanged(int);					 //�༭:Ai�ȼ��䶯
};
