#pragma once
#include "PositionStruct.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <qobject.h>
/*
 * ����ʵ��
 */
static PositionStruct situation;
/*
 * �������йص�ȫ�ֱ���
 */	 
static struct {
	int mvResult;             // �����ߵ���
	int nHistoryTable[GRID_COUNT]; // ��ʷ��
} Search;
/*
 * qsort����������ʷ������ıȽϺ���
 */
static int CompareHistory(const void *lpmv1, const void *lpmv2) {
	return Search.nHistoryTable[*(int *)lpmv2] - Search.nHistoryTable[*(int *)lpmv1];
}



class AlphaBetaRobot:public QObject
{
	Q_OBJECT

public:
	AlphaBetaRobot();
	~AlphaBetaRobot();
	
	void SearchMain(void);
	int SearchFull(int vlAlpha, int vlBeta, int nDepth);
public slots:
	void getAiResponse(POINT*);
signals:
	void AIComplete(POINT*);//��AI�����ӵ㷵��,�������
};