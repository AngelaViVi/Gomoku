#include "AlphaBetaRobot.h"
#include <qDebug>


AlphaBetaRobot::AlphaBetaRobot()
{
	situation.Startup();
	for (int i=0;i<GRID_COUNT;i++)
	{
		Search.nHistoryTable[i] = 0;
	}
}


AlphaBetaRobot::~AlphaBetaRobot()
{
}


// �����߽�(Fail-Soft)��Alpha-Beta��������
int AlphaBetaRobot::SearchFull(int vlAlpha, int vlBeta, int nDepth) {
	//qDebug() << "---------->(" << vlAlpha << "," << vlBeta << "," << nDepth << ")<----------" << "call SearchFull";
	int pcCaptured;
	int vl, vlBest, mvBest;
	int mvs[GRID_COUNT];//���н����������ܶ��������ϵĽǵ�����
						// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�
						// 1. ����ˮƽ�ߣ��򷵻ؾ�������ֵ
	if (nDepth == 0) {
		int temp= situation.Evaluate();
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "ready to return by depth = 0,situtation.nDistance= " << situation.nDistance << "  Evaluate()= " << temp;
		return temp;
	}

	// 2. ��ʼ�����ֵ������߷�
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	mvBest = 0;           // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��

						  // 3. ����ȫ���߷�����������ʷ������
	int nGenMoves = situation.GenerateMoves(mvs);
	qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
	//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "situation.GenerateMoves() = " << nGenMoves;
	// 4. ��һ����Щ�߷��������еݹ�
	for (int i = 0; i < nGenMoves; i++) {
		situation.MakeMove(mvs[i]);
		vl = -SearchFull(-vlBeta, -vlAlpha, nDepth - 1);
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "  vl=  " << vl;
		situation.UndoMakeMove(mvs[i]);
		// 5. ����Alpha-Beta��С�жϺͽض�
		if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
			vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
			if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
				mvBest = mvs[i];  // Beta�߷�Ҫ���浽��ʷ��
				/*Search.nHistoryTable[mvs[i]] += nDepth * nDepth;
				return vlBest;*/
				//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") Beat-Cut,mvBest=   " << mvBest;
				break;            // Beta�ض�
			}
			if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
				mvBest = mvs[i];  // PV�߷�Ҫ���浽��ʷ��
				vlAlpha = vl;     // ��СAlpha-Beta�߽�
				//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") PV,mvBest=   " << mvBest;
			}
		}

	}

	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (vlBest == -MATE_VALUE) {
		// �����vlBestû�б���ֵ��(û�������κ��߷�)
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "return by dead,value=  "<< situation.nDistance - MATE_VALUE;
		return situation.nDistance - MATE_VALUE;
	}
	//Search.nHistoryTable[mvBest] += nDepth * nDepth;
		
		
	if (mvBest != 0) {
		// �������Alpha�߷����ͽ�����߷����浽��ʷ��
		Search.nHistoryTable[mvBest] += nDepth * nDepth;
		if (situation.nDistance == 0) {
			//�������ڵ�ʱ��������һ������߷�(��Ϊȫ�����������ᳬ���߽�)��������߷���������
			Search.mvResult = mvBest;
			//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") update Search.mvResult to: " << mvBest;
		}
	}
	return vlBest;
}

// ����������������
void AlphaBetaRobot::SearchMain(void) {
	int i, t, vl;

	// ��ʼ��
	memset(Search.nHistoryTable, 0, 225 * sizeof(int)); // �����ʷ��
	t = clock();       // ��ʼ����ʱ��
	situation.nDistance = 0; // ��ʼ����
	//qDebug() << "======begin to Search by alpha beta======";
	//SearchFull(-100000, 100000, 4);
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

void AlphaBetaRobot::getAiResponse(POINT * playerPos)
{
	situation.MakeMove(playerPos->y*GRID_NUM + playerPos->x);
	//situation.ucpcSquares[playerPos->x*GRID_NUM + playerPos->y] = 0;//���Ĭ�������
	//situation.ChangeSide();
	SearchMain();
	situation.MakeMove(Search.mvResult);
	qDebug() << "ai respone in :" << Search.mvResult;
	POINT *pos = new POINT;
	pos->y = Search.mvResult / GRID_NUM;
	pos->x = Search.mvResult - pos->y*GRID_NUM;
	emit AIComplete(pos);
}