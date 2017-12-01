#include <cstdlib>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"StaticDefinition.h"
#include"Alpha-Beta.h"

#include <qDebug>
/*
* ��ֵ����
* ����1:λ��
* ����2:true=����,false=����
*/
int AiAgent::Eveluate(unsigned char position[][GRID_NUM], bool bIsWhiteTurn)
{
	int i, j, k;
	unsigned char nStoneType;
	count++;//�������ۼ�
			/*
			* void *memset(void *s, int ch, size_t n)
			* �������ͣ���s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s ��
			* memset����������һ���ڴ�������ĳ��������ֵ�����ǶԽϴ�Ľṹ�������������������һ����췽��
			*/
			//������ͷ������
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);

	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)//��ǰ������λ��������
			{
				//���ˮƽ������û�з�����
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//�����ֱ������û�з�����
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//�Է����������ͳ��,�õ�ÿ�����͵�����
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
			for (k = 0; k<4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != space)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://����
						TypeCount[nStoneType][FIVE]++;
						break;
					case FOUR://����
						TypeCount[nStoneType][FOUR]++;
						break;
					case SFOUR://����
						TypeCount[nStoneType][SFOUR]++;
						break;
					case THREE://����
						TypeCount[nStoneType][THREE]++;
						break;
					case STHREE://����
						TypeCount[nStoneType][STHREE]++;
						break;
					case TWO://���
						TypeCount[nStoneType][TWO]++;
						break;
					case STWO://�߶�
						TypeCount[nStoneType][STWO]++;
						break;
					default:
						break;
					}
				}
			}

	//���������,���ؼ�ֵ
	if (bIsWhiteTurn)
	{
		if (TypeCount[black][FIVE])
		{
			return -9999;
		}
		if (TypeCount[white][FIVE])
		{
			return 9999;
		}
	}
	else
	{
		if (TypeCount[black][FIVE])
		{
			return 9999;
		}
		if (TypeCount[white][FIVE])
		{
			return -9999;
		}
	}
	//�������ĵ���һ������
	if (TypeCount[white][SFOUR]>1)
		TypeCount[white][FOUR]++;
	if (TypeCount[black][SFOUR]>1)
		TypeCount[black][FOUR]++;
	int WValue = 0, BValue = 0;

	if (bIsWhiteTurn)//�ֵ�������
	{
		if (TypeCount[white][FOUR])
		{


			return 9990;//����,��ʤ���ؼ�ֵ
		}
		if (TypeCount[white][SFOUR])
		{


			return 9980;//����,��ʤ���ؼ�ֵ
		}
		if (TypeCount[black][FOUR])
		{


			return -9970;//���޳��Ļ���,�����л���,��ʤ���ؼ�ֵ
		}
		if (TypeCount[black][SFOUR] && TypeCount[black][THREE])
		{


			return -9960;//�����г��ĺͻ���,��ʤ���ؼ�ֵ
		}
		if (TypeCount[white][THREE] && TypeCount[black][SFOUR] == 0)
		{


			return 9950;//���л�������û����,��ʤ���ؼ�ֵ
		}
		if (TypeCount[black][THREE]>1 && TypeCount[white][SFOUR] == 0 && TypeCount[white][THREE] == 0 && TypeCount[white][STHREE] == 0)
		{


			return -9940;//�ڵĻ�������һ��,�������ĺ���,��ʤ���ؼ�ֵ
		}
		if (TypeCount[white][THREE]>1)
			WValue += 2000;//�׻�������һ��,�����ֵ��2000
		else
			//��������ֵ��200
			if (TypeCount[white][THREE])
				WValue += 200;
		if (TypeCount[black][THREE]>1)
			BValue += 500;//�ڵĻ�������һ��,�����ֵ��500
		else
			//��������ֵ��100
			if (TypeCount[black][THREE])
				BValue += 100;
		//ÿ��������10
		if (TypeCount[white][STHREE])
			WValue += TypeCount[white][STHREE] * 10;
		//ÿ��������10
		if (TypeCount[black][STHREE])
			BValue += TypeCount[black][STHREE] * 10;
		//ÿ�������4
		if (TypeCount[white][TWO])
			WValue += TypeCount[white][TWO] * 4;
		//ÿ�������4
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][TWO] * 4;
		//ÿ���߶���1
		if (TypeCount[white][STWO])
			WValue += TypeCount[white][STWO];
		//ÿ���߶���1
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][STWO];
	}
	else//�ֵ�������
	{
		if (TypeCount[black][FOUR])
		{

			return 9990;//����,��ʤ���ؼ�ֵ
		}
		if (TypeCount[black][SFOUR])
		{

			return 9980;//����,��ʤ���ؼ�ֵ
		}
		if (TypeCount[white][FOUR])
			return -9970;//����,��ʤ���ؼ�ֵ

		if (TypeCount[white][SFOUR] && TypeCount[white][THREE])
			return -9960;//���Ĳ�����,��ʤ���ؼ�ֵ

		if (TypeCount[black][THREE] && TypeCount[white][SFOUR] == 0)
			return 9950;//�ڻ���,�����ġ���ʤ���ؼ�ֵ

		if (TypeCount[white][THREE]>1 && TypeCount[black][SFOUR] == 0 && TypeCount[black][THREE] == 0 && TypeCount[black][STHREE] == 0)
			return -9940;//�׵Ļ�������һ��,�������ĺ���,��ʤ���ؼ�ֵ

						 //�ڵĻ�������һ��,�����ֵ��2000
		if (TypeCount[black][THREE]>1)
			BValue += 2000;
		else
			//��������ֵ��200
			if (TypeCount[black][THREE])
				BValue += 200;

		//�׵Ļ�������һ��,�����ֵ�� 500
		if (TypeCount[white][THREE]>1)
			WValue += 500;
		else
			//��������ֵ��100
			if (TypeCount[white][THREE])
				WValue += 100;

		//ÿ��������10
		if (TypeCount[white][STHREE])
			WValue += TypeCount[white][STHREE] * 10;
		//ÿ��������10
		if (TypeCount[black][STHREE])
			BValue += TypeCount[black][STHREE] * 10;

		//ÿ�������4
		if (TypeCount[white][TWO])
			WValue += TypeCount[white][TWO] * 4;
		//ÿ�������4
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][TWO] * 4;

		//ÿ���߶���1
		if (TypeCount[white][STWO])
			WValue += TypeCount[white][STWO];
		//ÿ���߶���1
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][STWO];
	}

	//�����������ӵ�λ�ü�ֵ
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			nStoneType = position[i][j];
			if (nStoneType != space)
				if (nStoneType == black)
					BValue += PosValue[i][j];
				else
					WValue += PosValue[i][j];
		}

	//���ع�ֵ
	if (!bIsWhiteTurn)
		return BValue - WValue;
	else
		return WValue - BValue;
}
/*
 *����������ĳ����ˮƽ�����ϵ�����
 */
int AiAgent::AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j)
{
	//����ֱ�߷�����������
	AnalysisLine(position[i], 15, j);
	//ʰȡ�������
	for (int s = 0; s<15; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];

	return TypeRecord[i][j][0];
}
/*
 *����������ĳ���ڴ�ֱ�����ϵ�����
 */
int AiAgent::AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	//����ֱ�����ϵ�����ת��һά����
	for (int k = 0; k<GRID_NUM; k++)
		tempArray[k] = position[k][j];
	//����ֱ�߷�����������
	AnalysisLine(tempArray, GRID_NUM, i);
	//ʰȡ�������
	for (int s = 0; s<GRID_NUM; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[s][j][1] = m_LineRecord[s];

	return TypeRecord[i][j][1];
}
/*
 *����������ĳ������б�����ϵ�����
 */
int AiAgent::AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y;
	int k;
	if (i<j)
	{
		y = 0;
		x = j - i;
	}
	else
	{
		x = 0;
		y = i - j;
	}
	//��б�����ϵ�����ת��һά����
	for (k = 0; k<GRID_NUM; k++)
	{
		if (x + k>14 || y + k>14)
			break;
		tempArray[k] = position[y + k][x + k];
	}
	//����ֱ�߷�����������
	AnalysisLine(tempArray, k, j - x);
	//ʰȡ�������
	for (int s = 0; s<k; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y + s][x + s][2] = m_LineRecord[s];

	return TypeRecord[i][j][2];
}
/*
 *����������ĳ������б�����ϵ�����
 */
int AiAgent::AnalysisRight(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y, realnum;
	int k;
	if (14 - i<j)
	{
		y = 14;
		x = j - 14 + i;
		realnum = 14 - i;
	}
	else
	{
		x = 0;
		y = i + j;
		realnum = j;
	}
	//��б�����ϵ�����ת��һά����
	for (k = 0; k<GRID_NUM; k++)
	{
		if (x + k>14 || y - k<0)
			break;
		tempArray[k] = position[y - k][x + k];
	}
	//����ֱ�߷�����������
	AnalysisLine(tempArray, k, j - x);
	//ʰȡ�������
	for (int s = 0; s<k; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y - s][x + s][3] = m_LineRecord[s];

	return TypeRecord[i][j][3];
}
/*
 *ֱ�߷���
 */
int AiAgent::AnalysisLine(unsigned char* position, int GridNum, int StonePos)
{
	unsigned char StoneType;
	unsigned char AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;

	if (GridNum<5)
	{
		//���鳤��С��5û������
		memset(m_LineRecord, ANALSISED, GridNum);
		return 0;
	}
	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, 30);
	memset(AnalyLine, 0x0F, 30);
	//����������װ��AnalyLine;
	memcpy(&AnalyLine, position, GridNum);
	GridNum--;
	StoneType = AnalyLine[nAnalyPos];
	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;
	//������������߽�
	while (LeftEdge>0)
	{
		if (AnalyLine[LeftEdge - 1] != StoneType)
			break;
		LeftEdge--;
	}

	//�����������ұ߽�
	while (RightEdge<GridNum)
	{
		if (AnalyLine[RightEdge + 1] != StoneType)
			break;
		RightEdge++;
	}
	LeftRange = LeftEdge;
	RightRange = RightEdge;
	//��������ѭ��������ӿ��µķ�Χ
	while (LeftRange>0)
	{
		if (AnalyLine[LeftRange - 1] == !StoneType)
			break;
		LeftRange--;
	}
	while (RightRange<GridNum)
	{
		if (AnalyLine[RightRange + 1] == !StoneType)
			break;
		RightRange++;
	}
	//����˷�ΧС��4�����û������
	if (RightRange - LeftRange<4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			m_LineRecord[k] = ANALSISED;
		return false;
	}
	//������������Ϊ��������,��ֹ�ظ�������һ����
	for (int k = LeftEdge; k <= RightEdge; k++)
		m_LineRecord[k] = ANALSISED;
	if (RightEdge - LeftEdge>3)
	{
		//���������������Ϊ����
		m_LineRecord[nAnalyPos] = FIVE;
		return FIVE;
	}

	if (RightEdge - LeftEdge == 3)
	{
		//���������������Ϊ����
		bool Leftfour = false;
		if (LeftEdge>0)
			if (AnalyLine[LeftEdge - 1] == space)
				Leftfour = true;//�������

		if (RightEdge<GridNum)
			//�ұ�δ���߽�
			if (AnalyLine[RightEdge + 1] == space)
				//�ұ�����
				if (Leftfour == true)//���������
					m_LineRecord[nAnalyPos] = FOUR;//����
				else
					m_LineRecord[nAnalyPos] = SFOUR;//����
			else
				if (Leftfour == true)//���������
					m_LineRecord[nAnalyPos] = SFOUR;//����
				else
					if (Leftfour == true)//���������
						m_LineRecord[nAnalyPos] = SFOUR;//����

		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 2)
	{
		//���������������Ϊ����
		bool LeftThree = false;

		if (LeftEdge>1)
			if (AnalyLine[LeftEdge - 1] == space)
				//�������
				if (LeftEdge>1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					//��߸�һ�հ��м�������
					m_LineRecord[LeftEdge] = SFOUR;//����
					m_LineRecord[LeftEdge - 2] = ANALSISED;
				}
				else
					LeftThree = true;

		if (RightEdge<GridNum)
			if (AnalyLine[RightEdge + 1] == space)
				//�ұ�����
				if (RightEdge<GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					//�ұ߸�1����������
					m_LineRecord[RightEdge] = SFOUR;//����
					m_LineRecord[RightEdge + 2] = ANALSISED;
				}
				else
					if (LeftThree == true)//���������
						m_LineRecord[RightEdge] = THREE;//����
					else
						m_LineRecord[RightEdge] = STHREE; //����
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)//�������
					return m_LineRecord[LeftEdge];//����

				if (LeftThree == true)//���������
					m_LineRecord[nAnalyPos] = STHREE;//����
			}
		else
		{
			if (m_LineRecord[LeftEdge] == SFOUR)//�������
				return m_LineRecord[LeftEdge];//����
			if (LeftThree == true)//���������
				m_LineRecord[nAnalyPos] = STHREE;//����
		}

		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 1)
	{
		//���������������Ϊ����
		bool Lefttwo = false;
		bool Leftthree = false;

		if (LeftEdge>2)
			if (AnalyLine[LeftEdge - 1] == space)
				//�������
				if (LeftEdge - 1>1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
					if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge])
					{
						//��߸�2����������
						m_LineRecord[LeftEdge - 3] = ANALSISED;
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = SFOUR;//����
					}
					else
						if (AnalyLine[LeftEdge - 3] == space)
						{
							//��߸�1����������
							m_LineRecord[LeftEdge - 2] = ANALSISED;
							m_LineRecord[LeftEdge] = STHREE;//����
						}
						else
							Lefttwo = true;

		if (RightEdge<GridNum - 2)
			if (AnalyLine[RightEdge + 1] == space)
				//�ұ�����
				if (RightEdge + 1<GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
					if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
					{
						//�ұ߸�������������
						m_LineRecord[RightEdge + 3] = ANALSISED;
						m_LineRecord[RightEdge + 2] = ANALSISED;
						m_LineRecord[RightEdge] = SFOUR;//����
					}
					else
						if (AnalyLine[RightEdge + 3] == space)
						{
							//�ұ߸� 1 ����������
							m_LineRecord[RightEdge + 2] = ANALSISED;
							m_LineRecord[RightEdge] = STHREE;//����
						}
						else
						{
							if (m_LineRecord[LeftEdge] == SFOUR)//��߳���
								return m_LineRecord[LeftEdge];//����

							if (m_LineRecord[LeftEdge] == STHREE)//�������        
								return m_LineRecord[LeftEdge];

							if (Lefttwo == true)
								m_LineRecord[nAnalyPos] = TWO;//���ػ��
							else
								m_LineRecord[nAnalyPos] = STWO;//�߶�
						}
				else
				{
					if (m_LineRecord[LeftEdge] == SFOUR)//���ķ���
						return m_LineRecord[LeftEdge];

					if (Lefttwo == true)//�߶�
						m_LineRecord[nAnalyPos] = STWO;
				}

		return m_LineRecord[nAnalyPos];
	}

	return 0;
}

//����ʷ��¼����������Ŀȫ��Ϊ��ֵ
void AiAgent::ResetHistoryTable()
{
	memset(m_HistoryTable, 10, GRID_COUNT * sizeof(int));
}

//����ʷ�÷ֱ���ȡ�����߷�����ʷ�÷�
int AiAgent::GetHistoryScore(STONEMOVE* move)
{
	return m_HistoryTable[move->StonePos.x][move->StonePos.y];
}

//��һ����߷�������ʷ��¼
void AiAgent::EnterHistoryScore(STONEMOVE* move, int depth)
{
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += 2 << depth;
}

//���߷����д�С��������
//STONEMOVE* sourceԭʼ����
//STONEMOVE* targetĿ�����
//�ϲ�source[l��m]�� source[m +1��r]��target[l��r]
void AiAgent::Merge(STONEMOVE* source, STONEMOVE* target, int l, int m, int r)
{
	//��С��������
	int i = l;
	int j = m + 1;
	int k = l;
	while (i <= m && j <= r)
		if (source[i].Score <= source[j].Score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];
	if (i>m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

void AiAgent::Merge_A(STONEMOVE* source, STONEMOVE* target, int l, int m, int r)
{
	//�Ӵ�С����
	int i = l;
	int j = m + 1;
	int k = l;
	while (i <= m &&j <= r)
		if (source[i].Score >= source[j].Score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];
	if (i>m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

//�ϲ���СΪ S ������������
//direction �Ǳ�־,ָ���ǴӴ�С���Ǵ�С��������
void AiAgent::MergePass(STONEMOVE* source, STONEMOVE* target, const int s, const int n, const bool direction)
{
	int i = 0;
	while (i <= n - 2 * s)
	{
		//�ϲ���СΪ s�����ڶ���������
		if (direction)
			Merge(source, target, i, i + s - 1, i + 2 * s - 1);
		else
			Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);
		i = i + 2 * s;
	}
	if (i + s<n)//ʣ���Ԫ�ظ���С��2s
	{
		if (direction)
			Merge(source, target, i, i + s - 1, n - 1);
		else
			Merge_A(source, target, i, i + s - 1, n - 1);
	}
	else
		for (int j = i; j <= n - 1; j++)
			target[j] = source[j];
}
/*
 * �鲢����
 */
void AiAgent::MergeSort(STONEMOVE* source, int n, bool direction)
{
	int s = 1;
	while (s<n)
	{
		MergePass(source, m_TargetBuff, s, n, direction);
		s += s;
		MergePass(m_TargetBuff, source, s, n, direction);
		s += s;
	}
}
/*
 * ��ȡ������н�
 */
int AiAgent::CreatePossibleMove(unsigned char position[][GRID_NUM], int nPly, int nSide)
{
	int i, j;
	m_nMoveCount = 0;
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] == (unsigned char)space)
				AddMove(j, i, nPly);
		}

	//ʹ����ʷ�������еľ�̬�鲢���������߷����н�������
	//����Ϊ����߼�֦Ч��
	//        CHistoryHeuristic history;
	MergeSort(m_MoveList[nPly], m_nMoveCount, 0);

	return m_nMoveCount;//���غϷ��߷�����
}

//��m_MoveList�в���һ���߷�
//nToX��Ŀ��λ�ú�����
//nToY��Ŀ��λ��������
//nPly�Ǵ��߷����ڵĲ��
int AiAgent::AddMove(int nToX, int nToY, int nPly)
{
	m_MoveList[nPly][m_nMoveCount].StonePos.x = nToX;
	m_MoveList[nPly][m_nMoveCount].StonePos.y = nToY;

	m_nMoveCount++;
	m_MoveList[nPly][m_nMoveCount].Score = PosValue[nToY][nToX];//ʹ��λ�ü�ֵ��������ǰ�߷��ļ�ֵ
	return m_nMoveCount;
}


/*
 * ʹ�æ�-������һ����Ѿ���
 */
void AiAgent::SearchAGoodMove(unsigned char position[][GRID_NUM], int Type)
{
	int Score;

	memcpy(CurPosition, position, GRID_COUNT);
	m_nMaxDepth = m_nSearchDepth;
	m_pTranspositionTable->CalculateInitHashKey(CurPosition);
	ResetHistoryTable();
	Score = NegaScout(m_nMaxDepth, -20000, 20000);
	X = m_cmBestMove.StonePos.y;
	Y = m_cmBestMove.StonePos.x;
	MakeMove(&m_cmBestMove, Type);
	memcpy(position, CurPosition, GRID_COUNT);
}
/*
 * �ж���Ϸ�Ƿ����
 */
int AiAgent::IsGameOver(unsigned char position[][GRID_NUM], int nDepth)
{
	int score, i;//����Ҫ�µ�������ɫ
	i = (m_nMaxDepth - nDepth) % 2;
	score = Eveluate(position, i);//���ù�ֵ����
	if (abs(score)>8000)//�����ֵ�������ؼ�ֵ������������Ϸ����
		return score;//���ؼ�ֵ
	return 0;//����δ����
}
/*
 * ������ֵ���Ħ�-������
 */
int AiAgent::NegaScout(int depth, int alpha, int beta)
{
	int Count, i;
	unsigned char type;
	int a, b, t;
	int side;
	int score;
	/*        if(depth>0)
	{
	i= IsGameOver(CurPosition,depth);
	if(i!=0)
	return i;  //�ѷ�ʤ�������ؼ�ֵ
	}
	*/
	side = (m_nMaxDepth - depth) % 2;//���㵱ǰ�ڵ������,����0/��С1
	score = m_pTranspositionTable->LookUpHashTable(alpha, beta, depth, side);
	if (score != 66666)
		return score;
	if (depth <= 0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		score = Eveluate(CurPosition, side);
		m_pTranspositionTable->EnterHashTable(exact, score, depth, side);//����ֵ�����û���

		return score;
	}
	Count = CreatePossibleMove(CurPosition, depth, side);
	for (i = 0; i<Count; i++)
		m_MoveList[depth][i].Score = GetHistoryScore(&m_MoveList[depth][i]);

	MergeSort(m_MoveList[depth], Count, 0);
	int bestmove = -1;
	a = alpha;
	b = beta;

	int eval_is_exact = 0;

	for (i = 0; i<Count; i++)
	{
		type = MakeMove(&m_MoveList[depth][i], side);
		m_pTranspositionTable->Hash_MakeMove(&m_MoveList[depth][i], CurPosition);
		t = -NegaScout(depth - 1, -b, -a);//�ݹ������ӽڵ㣬�Ե� 1 ���ڵ���ȫ���ڣ�����ǿմ�̽��
		if (t>a && t<beta && i>0)
		{
			//���ڵ�һ����Ľڵ�,������������failhigh
			a = -NegaScout(depth - 1, -beta, -t);//re-search
			eval_is_exact = 1;//����������Ϊ��ȷֵ
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_MoveList[depth][i];
			bestmove = i;
		}
		m_pTranspositionTable->Hash_UnMakeMove(&m_MoveList[depth][i], CurPosition);
		UnMakeMove(&m_MoveList[depth][i]);
		if (a<t)
		{
			eval_is_exact = 1;
			a = t;
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_MoveList[depth][i];
		}
		if (a >= beta)
		{
			m_pTranspositionTable->EnterHashTable(lower_bound, a, depth, side);
			EnterHistoryScore(&m_MoveList[depth][i], depth);
			return a;
		}
		b = a + 1;                      /* set new null window */
	}
	if (bestmove != -1)
		EnterHistoryScore(&m_MoveList[depth][bestmove], depth);
	if (eval_is_exact)
		m_pTranspositionTable->EnterHashTable(exact, a, depth, side);
	else
		m_pTranspositionTable->EnterHashTable(upper_bound, a, depth, side);

	return a;
}
/*
 * ִ��һ������
 */
unsigned char AiAgent::MakeMove(STONEMOVE* move, int type)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = type;
	return 0;
}
/*
 * ����һ������
 */
void AiAgent::UnMakeMove(STONEMOVE* move)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = space;
}
/*
* ���캯��
*/
AiAgent::AiAgent()
{	
	m_pTranspositionTable = new TranspositionTable();
	colour = black;
	m_nUserStoneColor = white;
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
			m_RenjuBoard[i][j] = space; //���̳�ʼ�� 

}
/*
* ����������ж�,��ȡAiAgent���ж�
*/
void AiAgent::GetAiAction(POINT*PlayerPos)
{
	qDebug() << "GetAiAction is running...";
	int XS;
	POINT *temp=new POINT;
	if (XS == 1 && PlayerPos->x == -1 && PlayerPos->y == -1) //�������ִ�����ǵ�һ������ռ����������λ�� 
	{
		m_RenjuBoard[GRID_NUM / 2][GRID_NUM / 2] = colour; //����������Ϣ
	}
	else
	{
		m_RenjuBoard[PlayerPos->x][PlayerPos->y] = m_nUserStoneColor;
		m_nSearchDepth = 4;//AI�Ѷȵȼ����� 
						   //����������        
		ResetHistoryTable();
		m_pTranspositionTable->InitializeHashKey();//������ϣ�������������
		SearchAGoodMove(m_RenjuBoard, colour);
		m_RenjuBoard[X][Y] = colour;
		//��� 
		
		temp->x = X;
		temp->y = Y;
		std::cout << std::flush; //ˢ�»�����
		m_pTranspositionTable->_CTranspositionTable();
	}
	emit AIComplete(temp);//��AI�����ӵ㷢��
}