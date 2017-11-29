/*
 * ʹ�øĽ����alpha-beta��֦�㷨����������AiAgent
 */
#pragma once
#include "StaticDefinition.h"
#include "TranspositionTable.h"
class AiAgent
{
public:
	AiAgent();
	POINT GetAiAction(int PlayerX, int PlayerY);
private:
	TranspositionTable *m_pTranspositionTable;
	int m_HistoryTable[GRID_NUM][GRID_NUM];//��ʷ�÷ֱ�

	STONEMOVE m_TargetBuff[225];    //�����õĻ������
	STONEMOVE m_MoveList[10][225];//���Լ�¼�߷�������
	STONEMOVE m_cmBestMove;        //��¼����߷��ı���        
								   //CMoveGenerator* m_pMG;        //�߷�������ָ��        
								   //CEveluation* m_pEval;        //��ֵ����ָ��   
	unsigned char m_LineRecord[30];          //���AnalysisLine�������������
	unsigned char CurPosition[GRID_NUM][GRID_NUM];//����ʱ���ڵ�ǰ�ڵ�����״̬������
	unsigned char m_RenjuBoard[GRID_NUM][GRID_NUM];//�������飬������ʾ����


	int m_nMoveCount;//�˱������Լ�¼�߷�������
	int m_nSearchDepth;        //����������
	int m_nMaxDepth;        //��ǰ����������������
	int m_nUserStoneColor;         //�û����ӵ���ɫ
	int X, Y;                               //AI�������λ�� 			 
	int count = 0;//ȫ�ֱ���,����ͳ�ƹ�ֵ������ִ�б���
	int colour;
	int TypeRecord[GRID_NUM][GRID_NUM][4];//���ȫ���������������,������ά��,���ڴ��ˮƽ����ֱ����б����б 4 ���������������ͷ������
	int TypeCount[2][20];          //���ͳ�ǹ��ķ������������
	//λ����Ҫ�Լ�ֵ��,�˱���м�����,Խ�����ֵԽ��
	int PosValue[GRID_NUM][GRID_NUM] =
	{
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
	};

private:
	int AnalysisLine(unsigned char* position, int GridNum, int StonePos);
	int AnalysisRight(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j);
	int Eveluate(unsigned char position[][GRID_NUM], bool bIsWhiteTurn);
	int AddMove(int nToX, int nToY, int nPly);
	int CreatePossibleMove(unsigned char position[][GRID_NUM], int nPly, int nSide);
	void MergeSort(STONEMOVE* source, int n, bool direction);
	void MergePass(STONEMOVE* source, STONEMOVE* target, const int s, const int n, const bool direction);
	void Merge_A(STONEMOVE* source, STONEMOVE* target, int l, int m, int r);
	void Merge(STONEMOVE* source, STONEMOVE* target, int l, int m, int r);
	void EnterHistoryScore(STONEMOVE* move, int depth);
	int GetHistoryScore(STONEMOVE* move);
	void ResetHistoryTable();
	int NegaScout(int depth, int alpha, int beta);
	void SearchAGoodMove(unsigned char position[][GRID_NUM], int Type);
	int IsGameOver(unsigned char position[][GRID_NUM], int nDepth);
	void UnMakeMove(STONEMOVE* move);
	unsigned char MakeMove(STONEMOVE* move, int type);
};