#include "Robot.h"
#include <qDebug>


Robot::Robot()
{
	situation.Startup();
	for (int i=0;i<GRID_COUNT;i++)
	{
		Search.nHistoryTable[i] = 0;
	}
}


Robot::~Robot()
{
}


// �����߽�(Fail-Soft)��Alpha-Beta��������
int Robot::SearchFull(int vlAlpha, int vlBeta, int nDepth) {
	int i, nGenMoves, pcCaptured;
	int vl, vlBest, mvBest;
	int mvs[GRID_COUNT];//���н����������ܶ��������ϵĽǵ�����
						// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�

						// 1. ����ˮƽ�ߣ��򷵻ؾ�������ֵ
	if (nDepth == 0) {
		return situation.Evaluate();
	}

	// 2. ��ʼ�����ֵ������߷�
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	mvBest = 0;           // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��

						  // 3. ����ȫ���߷�����������ʷ������
	nGenMoves = situation.GenerateMoves(mvs);
	qsort(mvs, nGenMoves, sizeof(int), CompareHistory);

	// 4. ��һ����Щ�߷��������еݹ�
	for (i = 0; i < nGenMoves; i++) {
		situation.MakeMove(mvs[i]);
		vl = -SearchFull(-vlBeta, -vlAlpha, nDepth - 1);
		situation.UndoMakeMove(mvs[i]);
		// 5. ����Alpha-Beta��С�жϺͽض�
		if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
			vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
			if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
				mvBest = mvs[i];  // Beta�߷�Ҫ���浽��ʷ��
				break;            // Beta�ض�
			}
			if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
				mvBest = mvs[i];  // PV�߷�Ҫ���浽��ʷ��
				vlAlpha = vl;     // ��СAlpha-Beta�߽�
			}
		}

	}

	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (vlBest == -MATE_VALUE) {
		// �����ɱ�壬�͸���ɱ�岽����������
		return situation.nDistance - MATE_VALUE;
	}
	if (mvBest != 0) {
		// �������Alpha�߷����ͽ�����߷����浽��ʷ��
		Search.nHistoryTable[mvBest] += nDepth * nDepth;
		if (situation.nDistance == 0) {
			// �������ڵ�ʱ��������һ������߷�(��Ϊȫ�����������ᳬ���߽�)��������߷���������
			Search.mvResult = mvBest;
		}
	}
	return vlBest;
}

// ����������������
void Robot::SearchMain(void) {
	int i, t, vl;

	// ��ʼ��
	memset(Search.nHistoryTable, 0, 65536 * sizeof(int)); // �����ʷ��
	t = clock();       // ��ʼ����ʱ��
	situation.nDistance = 0; // ��ʼ����
	//SearchFull(-MATE_VALUE, MATE_VALUE, 4);
							 // �����������
	for (i = 1; i <= LIMIT_DEPTH; i++) {
		vl = SearchFull(-MATE_VALUE, MATE_VALUE, i);
		 //��ɱ��ֹ
		if (vl > 100000 || vl < -100000) {//
			break;
		}
		 //��ʱ��ֹ
		if (clock() - t > CLOCKS_PER_SEC) {
			break;
		}
	}   // ��Ⱥľ���ֹ
}

void Robot::getAiResponse(POINT * playerPos)
{
	situation.MakeMove(playerPos->x*GRID_NUM + playerPos->y);
	//situation.ucpcSquares[playerPos->x*GRID_NUM + playerPos->y] = 0;//���Ĭ�������
	//situation.ChangeSide();
	SearchMain();
	situation.MakeMove(Search.mvResult);
	qDebug() << "Search.mvResult:" << Search.mvResult;
	POINT *pos = new POINT;
	pos->x = Search.mvResult / GRID_NUM;
	pos->y = Search.mvResult - pos->x*GRID_NUM;
	emit AIComplete(pos);
}