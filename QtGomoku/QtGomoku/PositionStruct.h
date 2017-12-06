#pragma once
#include "StaticDefinition.h"

struct PositionStruct {
	int sdPlayer;                               // �ֵ�˭�ߣ�0=���壬1=����(0����)
	int ucpcSquares[GRID_COUNT];                // ����
	int vlWhite, vlBlack;                       // ˫�����ܼ�ֵ
	int nDistance;                              // ��ǰ����ǴӸ��ڵ�����¼��������
	int TypeRecord[GRID_NUM][GRID_NUM][4];		// TypeRecord[x][y][i]��ʾ����xy��i�����ϵ����η������
												// i:0=ˮƽ,1==��ֱ,2==������,3==�η���
	int TypeCount[2][20];						// ���ͳ�ǹ��ķ������������,TypeCount[i][j]Ԫ�ر�ʾ��i����ɫ�����ӵĵ�j�����ε�����
	int m_LineRecord[30];						// ���AnalysisLine�������������

	void Startup(void);                         // ��ʼ������
	void ChangeSide(void);			            // �������ӷ�
	void AddPiece(int pos, int color);          // color����������(0��1��),pos��λ��
	void DelPiece(int pos, int color);          // ������������һö����
	int Evaluate(void);							// ��ֵ����
	void MakeMove(int mv);						// ��һ����
	void UndoMakeMove(int mv);					// ������һ����
	int GenerateMoves(int *mvs) ;				// ���������߷�
	bool LegalMove(int mv) const;               // �ж��߷��Ƿ����

	int NeighborSum(int pos);					//��ĳ���ӵ������
	int AnalysisLine(int* position, int GridNum, int StonePos);
	int AnalysisSubDiagonal(int position[][GRID_NUM], int i, int j);
	int AnalysisMainDiagonal(int position[][GRID_NUM], int i, int j);
	int AnalysisVertical(int position[][GRID_NUM], int i, int j);
	int Analysis_horizon(int position[][GRID_NUM], int i, int j);
};