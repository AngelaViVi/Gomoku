#include <cstdlib>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"StaticDefinition.h"
#include"Alpha-Beta.h"

#include <qDebug>
#include <valarray>

/*
* ��ֵ����
* ����1:Ҫ���۵ľ���(�洢���������һ������)
* ����2:�ҷ�����ɫ
* 1.��ֵ�����ھ���ʱ��Ҷ�ڵ㱻����
* 2.��ֵ�������۵���Ҷ�ڵ�������Ƶĺû�
* 3.����ԭ��:
*			������ֱ�ʤ����,��ǰֹͣ�ж�,ֱ�ӷ���һ���ϴ�Ĺ�ֵ
*			���㵱ǰ�����ҷ�����������
*			���㵱ǰ����Է�����������
*			����:�ҷ�����-�Է�����(������ѡ����ҷ�������,�ԶԷ������·��)
*/
int AiAgent::Eveluate(unsigned char position[][GRID_NUM], Color color)
{
	count++;//�������ۼ�
	/*
	* void *memset(void *s, int ch, size_t n)
	* �������ͣ���s�е�ǰλ�ú����n���ֽ� ��typedef unsigned int size_t ���� ch �滻������ s ��
	* memset����������һ���ڴ�������ĳ��������ֵ�����ǶԽϴ�Ľṹ�������������������һ����췽��
	*/
	//������ͷ������
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);

	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)//��ǰ������λ��������
			{
				//���ˮƽ������û�з�����
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//�����ֱ������û�з�����
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//������Խ��߷�����û�з�����
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisMainDiagonal(position, i, j);

				//����ζԽ��߷�����û�з�����
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisSubDiagonal(position, i, j);
			}
		}
	}
		

	//�Է����������ͳ��,�õ�ÿ�����͵�����
	//���ַ�����һ������:���ڵ�һ������,��ͬһ�����ϲ�����ͬʱ�γ����ֻ�������������
	//����������費����,������ֵ����ʧЧ
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			for (int k = 0; k<4; k++)
			{
				if (position[i][j] != space)
				{
					switch (TypeRecord[i][j][k])
					{
					case Five://����
						TypeCount[position[i][j]][Five]++;
						break;
					case Alive4://����
						TypeCount[position[i][j]][Alive4]++;
						break;
					case Sleep4://����
						TypeCount[position[i][j]][Sleep4]++;
						break;
					case Alive3://����
						TypeCount[position[i][j]][Alive3]++;
						break;
					case Sleep3://����
						TypeCount[position[i][j]][Sleep3]++;
						break;
					case Alive2://���
						TypeCount[position[i][j]][Alive2]++;
						break;
					case Sleep2://�߶�
						TypeCount[position[i][j]][Sleep2]++;
						break;
					default:
						break;
					}
				}
			}
		}
	}
	//========��ʼ����========
	int _ourScore = 0, _counterScore = 0;//�ҷ�����,�Է�����
	Color _ourSide = color;//�ҷ���ɫ
	Color _counterSide = (color == white ? black : white);//�Է���ɫ
	/*
	 *��ʤ/�ذ�ֻ���������:
	 * 1.����
	 * 2.����,˫����,���Ļ���
	 * ͬʱ�������������ʱ��,������ҷ�,����ѡ���γ�����
	 * ����ǶԷ�,������Ϊ����������ǻ���һ������
	 * 
	 * ���������������Ʒ�
	 */
	//����
	if (TypeCount[_ourSide][Five] >= 1)
	{
		return 100000;
	}
	if (TypeCount[_counterSide][Five] >= 1)
	{
		return -100000;
	}
	//����,˫����,���Ļ���
	if (TypeCount[_ourSide][Alive4]>=1||TypeCount[_ourSide][Sleep4]>=2||(TypeCount[_ourSide][Sleep4]>=1&& TypeCount[_ourSide][Alive3>=1]))
	{
		return 10000;
	}
	if (TypeCount[_counterSide][Alive4]>= 1||TypeCount[_counterSide][Sleep4]>= 2||(TypeCount[_counterSide][Sleep4]>=1&&TypeCount[_counterSide][Alive3 >= 1]))
	{
		return -10000;
	}
	//=====��ʼ���,ע�ⲻ�򸺷�====
	//˫����
	if (TypeCount[_ourSide][Alive3] >= 2)
	{
		_ourScore += 5000;
	}
	if (TypeCount[_counterSide][Alive3] >= 2)
	{
		_counterScore += 5000;
	}
	//��������
	if (TypeCount[_ourSide][Alive3] >= 1&& TypeCount[_ourSide][Sleep3]>=1)
	{
		_ourScore += 1000;
	}
	if (TypeCount[_counterSide][Alive3] >= 1 && TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += 1000;
	}
	//����
	if (TypeCount[_ourSide][Sleep4] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep4]*500;
	}
	if (TypeCount[_counterSide][Sleep4] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep4]*500;
	}
	//����
	if (TypeCount[_ourSide][Alive3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive3]*200;
	}
	if (TypeCount[_counterSide][Alive3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive3]*200;
	}
	//˫���
	if (TypeCount[_ourSide][Alive2] >= 2)
	{
		_ourScore += 100;
	}
	if (TypeCount[_counterSide][Alive2] >= 2)
	{
		_counterScore += 100;
	}
	//����
	if (TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep3]*50;
	}
	if (TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep3]*50;
	}
	//����߶�
	if (TypeCount[_ourSide][Alive2] >= 1&& TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += 10;
	}
	if (TypeCount[_counterSide][Alive2] >= 1&& TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += 10;
	}
	//���
	if (TypeCount[_ourSide][Alive2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive2]*5;
	}
	if (TypeCount[_counterSide][Alive2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive2]*5;
	}
	//�߶�
	if (TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep2]*3;
	}
	if (TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep2]*3;
	}
	/*
	 * ����,����,����,��һ
	 */
	//======������Ȩ�ط�=====
	//�����������ӵ�λ�ü�ֵ
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)
			{
				if (position[i][j] == _ourSide)
					_ourScore += PosValue[i][j];
				else
					_counterScore += PosValue[i][j];
			}
				
		}
	}
		
//================��ֽ���=========================
	return _ourScore - _counterScore;
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
 *����������ĳ�����Խ��߷����ϵ�����
 */
int AiAgent::AnalysisMainDiagonal(unsigned char position[][GRID_NUM], int i, int j)
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
 *����������ĳ���ڴζԽ��߷����ϵ�����
 */
int AiAgent::AnalysisSubDiagonal(unsigned char position[][GRID_NUM], int i, int j)
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
		m_LineRecord[nAnalyPos] = Five;
		return Five;
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
					m_LineRecord[nAnalyPos] = Alive4;//����
				else
					m_LineRecord[nAnalyPos] = Sleep4;//����
			else
				if (Leftfour == true)//���������
					m_LineRecord[nAnalyPos] = Sleep4;//����
				else
					if (Leftfour == true)//���������
						m_LineRecord[nAnalyPos] = Sleep4;//����

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
					m_LineRecord[LeftEdge] = Sleep4;//����
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
					m_LineRecord[RightEdge] = Sleep4;//����
					m_LineRecord[RightEdge + 2] = ANALSISED;
				}
				else
					if (LeftThree == true)//���������
						m_LineRecord[RightEdge] = Alive3;//����
					else
						m_LineRecord[RightEdge] = Sleep3; //����
			else
			{
				if (m_LineRecord[LeftEdge] == Sleep4)//�������
					return m_LineRecord[LeftEdge];//����

				if (LeftThree == true)//���������
					m_LineRecord[nAnalyPos] = Sleep3;//����
			}
		else
		{
			if (m_LineRecord[LeftEdge] == Sleep4)//�������
				return m_LineRecord[LeftEdge];//����
			if (LeftThree == true)//���������
				m_LineRecord[nAnalyPos] = Sleep3;//����
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
						m_LineRecord[LeftEdge] = Sleep4;//����
					}
					else
						if (AnalyLine[LeftEdge - 3] == space)
						{
							//��߸�1����������
							m_LineRecord[LeftEdge - 2] = ANALSISED;
							m_LineRecord[LeftEdge] = Sleep3;//����
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
						m_LineRecord[RightEdge] = Sleep4;//����
					}
					else
						if (AnalyLine[RightEdge + 3] == space)
						{
							//�ұ߸� 1 ����������
							m_LineRecord[RightEdge + 2] = ANALSISED;
							m_LineRecord[RightEdge] = Sleep3;//����
						}
						else
						{
							if (m_LineRecord[LeftEdge] == Sleep4)//��߳���
								return m_LineRecord[LeftEdge];//����

							if (m_LineRecord[LeftEdge] == Sleep3)//�������        
								return m_LineRecord[LeftEdge];

							if (Lefttwo == true)
								m_LineRecord[nAnalyPos] = Alive2;//���ػ��
							else
								m_LineRecord[nAnalyPos] = Sleep2;//�߶�
						}
				else
				{
					if (m_LineRecord[LeftEdge] == Sleep4)//���ķ���
						return m_LineRecord[LeftEdge];

					if (Lefttwo == true)//�߶�
						m_LineRecord[nAnalyPos] = Sleep2;
				}

		return m_LineRecord[nAnalyPos];
	}

	return 0;
}

//����ʷ��¼����������Ŀȫ��Ϊ��ֵ
void AiAgent::ResetHistoryTable()
{
	memset(m_HistoryTable, 0, GRID_COUNT * sizeof(int));
}

//����ʷ�÷ֱ���ȡ�����߷�����ʷ�÷�
int AiAgent::GetHistoryScore(STONEMOVE* move)
{
	return m_HistoryTable[move->StonePos.x][move->StonePos.y];
}

/*
 * ��һ����߷�������ʷ��¼,�߷�����ʷ���е�ֵ����ȵ�ƽ��
 */
void AiAgent::EnterHistoryScore(STONEMOVE* move, int depth)
{
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += 2 << depth;//������λ,��4
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += std::pow(depth, 2);
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
 * Ŀǰ��ȫ�ֱ���,���õķ����Ƕ������ӵĵط�������������
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
	Score = NegaScout(m_nMaxDepth, -100000, 100000);
	//Score=SearchFull(-MATE_VALUE, MATE_VALUE, Max_Level);
	X = m_cmBestMove.StonePos.y;
	Y = m_cmBestMove.StonePos.x;
	qDebug() << "Ai's Pos: (" << X << "," << Y << ")" << "  Score:" << m_cmBestMove.Score;
	MakeMove(&m_cmBestMove, Type);
	memcpy(position, CurPosition, GRID_COUNT);
}
/*
 * �ж���Ϸ�Ƿ����
 */
int AiAgent::IsGameOver(unsigned char position[][GRID_NUM], int nDepth)
{
	int score = Eveluate(position, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));//���ù�ֵ����
	if (abs(score)>8000)//�����ֵ�������ؼ�ֵ������������Ϸ����
		return score;//���ؼ�ֵ
	return 0;//����δ����
}
/*
 * ������ֵ���Ħ�-������
 * ����:depth:���������������
 *		alpha:��,�½�
 *		beat :��,�Ͻ�
 */
int AiAgent::NegaScout(int depth, int alpha, int beta)
{
	int vlBest = -MATE_VALUE;
	/*        if(depth>0)
	{
	i= IsGameOver(CurPosition,depth);
	if(i!=0)
	return i;  //�ѷ�ʤ�������ؼ�ֵ
	}
	*/
	int side = (m_nMaxDepth - depth) % 2;//���㵱ǰ�ڵ������,����0/��С1
	int score = m_pTranspositionTable->LookUpHashTable(alpha, beta, depth, side);
	if (score != 66666)
		return score;
	//====1=====�ݹ����:���ؾ����ֵ
	if (depth <= 0)//������ײ�(Ŀǰ�����������Ⱥľ�)
	{
		score = Eveluate(CurPosition, ((m_nMaxDepth - depth) % 2 == 0 ? black : white));
		//qDebug() << "Eveluate=" << score;
		m_pTranspositionTable->EnterHashTable(exact, score, depth, side);//����ֵ�����û���
		return score;
	}
	//====2=====���ɿ��н�,��������ʷ������
	int numOfPossibleMove = CreatePossibleMove(CurPosition, depth, side);//������н�����
	for (int i = 0; i<numOfPossibleMove; i++)
	{
		m_MoveList[depth][i].Score = GetHistoryScore(&m_MoveList[depth][i]);
		//qDebug() << "HistoryScore=" << m_MoveList[depth][i].Score;
	}
	MergeSort(m_MoveList[depth], numOfPossibleMove, 1);
	int bestmove = -1;
	int a = alpha;
	int b = beta;

	int eval_is_exact = 0;
	//====3=====�𲽳��Կ��н�,�����еݹ�
	for (int i = 0; i<numOfPossibleMove; i++)
	{
		unsigned char type = MakeMove(&m_MoveList[depth][i], side);
		m_pTranspositionTable->Hash_MakeMove(&m_MoveList[depth][i], CurPosition);
		int t = -NegaScout(depth - 1, -b, -a);//�ݹ������ӽڵ㣬�Ե� 1 ���ڵ���ȫ���ڣ�����ǿմ�̽��
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



// �����߽�(Fail-Soft)��Alpha-Beta��������
int AiAgent::SearchFull(int vlAlpha, int vlBeta, int nDepth) {
	int i, nGenMoves, pcCaptured;
	int vl, vlBest;
	STONEMOVE*mvBest = new STONEMOVE;
	//int mvs[GRID_COUNT];
	// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�

	// 1. ����ˮƽ�ߣ��򷵻ؾ�������ֵ
	if (nDepth == 0) {
		return  Eveluate(CurPosition, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));
	}

	// 2. ��ʼ�����ֵ������߷�
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	//mvBest = 0;           // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��

	// 3. ����ȫ���߷�����������ʷ������
	nGenMoves = CreatePossibleMove(CurPosition, nDepth, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));//������н�����
	//nGenMoves = pos.GenerateMoves(mvs);
	//������ʷ���CurPosition����=======================�˴���Ҫע��
	for (int i = 0; i<nGenMoves; i++)
	{
		m_MoveList[nDepth][i].Score = GetHistoryScore(&m_MoveList[nDepth][i]);
		//qDebug() << "HistoryScore=" << m_MoveList[depth][i].Score;
	}
	MergeSort(m_MoveList[nDepth], nGenMoves, 0);
	//qsort(m_MoveList[nDepth], nGenMoves, sizeof(STONEMOVE), CompareHistory);
	SortByHistory(m_MoveList[nDepth], nGenMoves);
	// 4. ��һ����Щ�߷��������еݹ�
	for (i = 0; i < nGenMoves; i++) {
		unsigned char type = MakeMove(&m_MoveList[nDepth][i], ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));
		vl = -SearchFull(-vlBeta, -vlAlpha, nDepth - 1);
		UnMakeMove(&m_MoveList[nDepth][i]);

		//pos.UndoMakeMove(mvs[i], pcCaptured);

		// 5. ����Alpha-Beta��С�жϺͽض�
		if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
			vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
			if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
				*mvBest = m_MoveList[nDepth][i];  // Beta�߷�Ҫ���浽��ʷ��
				break;            // Beta�ض�
			}
			if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
				*mvBest = m_MoveList[nDepth][i];  // PV�߷�Ҫ���浽��ʷ��
				vlAlpha = vl;     // ��СAlpha-Beta�߽�
			}
		}

	}

	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (vlBest == -MATE_VALUE) {
		// �����ɱ�壬�͸���ɱ�岽����������
		return (Max_Level-nDepth) - MATE_VALUE;
	}
	if (mvBest->StonePos.x == -1&& mvBest->StonePos.y == -1) {
		// �������Alpha�߷����ͽ�����߷����浽��ʷ��
		m_HistoryTable[(int)(mvBest->StonePos.x)][(int)(mvBest->StonePos.y)]+= nDepth * nDepth;
		if (nDepth == Max_Level) {
			// �������ڵ�ʱ��������һ������߷�(��Ϊȫ�����������ᳬ���߽�)��������߷���������
			//Search.mvResult = mvBest;
			m_cmBestMove = *mvBest;
		}
	}
	return vlBest;
}
/*
 * ������ʷ������
 */
void AiAgent::SortByHistory(STONEMOVE*list,int length)
{	 
	bool swaped;
	int n = length;
	do {
		swaped = false;
		for (int i = 1; i < n; i++) {
			if (m_HistoryTable[(int)(list[i].StonePos.x)][(int)(list[i].StonePos.y)]<m_HistoryTable[(int)(list[i].StonePos.x)][(int)(list[i].StonePos.y)]) {
				STONEMOVE temp = list[i - 1];
				list[i - 1] = list[i];
				list[i] = temp;
				swaped = true;
			}
		}
		n--;
	} while (swaped);
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