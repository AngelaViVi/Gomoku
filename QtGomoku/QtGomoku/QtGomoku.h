#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "Robot.h"


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
	//AiAgent *m_ai;
	Robot * robot;
	int Count;					//�������µ������ǵڼ���
public:
	Color HumanColor = black;	//����ִ������ɫ����
	Color AiColor = white;		//Aiִ������ɫ����
	Player CurrentTurn = Human; //��ǰ�ֵ�˭����
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
	void OnCheckedSoundOn();					 //��ѡ:��������
	void OnCheckedSoundOff();					 //��ѡ:�ر�����
	void OnCheckedPVE();						 //��ѡ:�˻���ս
	void OnCheckedPVP();						 //��ѡ:���˶�ս
};
