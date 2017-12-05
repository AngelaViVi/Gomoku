#include "PositionStruct.h"
#include <cstring>
#include <qDebug>

void PositionStruct::Startup(void)
{
	sdPlayer = 0;
	
	//memcpy(ucpcSquares, cucpcStartup, GRID_COUNT);
	for(int i=0;i<GRID_COUNT;i++)
	{
		ucpcSquares[i] = -1;
	}
}
/*
 * �������ӷ�
 */
void PositionStruct::ChangeSide()
{
	sdPlayer = 1 - sdPlayer;
}
/*
 * �������Ϸ�һö����
 * color����������(0��1��),pos��λ��
 */
void PositionStruct::AddPiece(int pos, int color)
{
	// �������Ϸ�һö����
	ucpcSquares[pos] = color;
	if (color == 1)
	{
		vlWhite += PosValue[pos];
	}
	else
	{
		vlBlack += PosValue[pos];
	}
}
/*
 *������������һö����
 *color����������(0��1��),pos��λ��
 */
void PositionStruct::DelPiece(int pos, int color)
{
	ucpcSquares[pos] = -1;
	if (color == 1)
	{
		vlWhite -= PosValue[pos];
	}
	else
	{
		vlBlack -= PosValue[pos];
	}
}
/*
 * �������ۺ���(���޸�)
 */
int PositionStruct::Evaluate()
{
	int position[GRID_NUM][GRID_NUM];
	//������תΪ��ά����
	for (int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			position[i][j] = ucpcSquares[j*15+i];
		}
	}
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
					Analysis_horizon(position, i, j);

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
	Color _ourSide = sdPlayer==0?black:white;//�ҷ���ɫ
	Color _counterSide = (_ourSide == white ? black : white);//�Է���ɫ
	//Color _ourSide = white;
	//Color _counterSide = black;
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
		return -99990;
	}
	//����,˫����,���Ļ���
	if (TypeCount[_ourSide][Alive4] >= 1 || TypeCount[_ourSide][Sleep4] >= 2 || (TypeCount[_ourSide][Sleep4] >= 1 && TypeCount[_ourSide][Alive3 >= 1]))
	{
		return 10000;
	}
	if (TypeCount[_counterSide][Alive4] >= 1 || TypeCount[_counterSide][Sleep4] >= 2 || (TypeCount[_counterSide][Sleep4] >= 1 && TypeCount[_counterSide][Alive3 >= 1]))
	{
		return -9990;
	}
	//=====��ʼ���,ע�ⲻ�򸺷�====
	//˫����
	if (TypeCount[_ourSide][Alive3] >= 2)
	{
		_ourScore += 5000;
	}
	if (TypeCount[_counterSide][Alive3] >= 2)
	{
		_counterScore += 4990;
	}
	//��������
	if (TypeCount[_ourSide][Alive3] >= 1 && TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += 1000;
	}
	if (TypeCount[_counterSide][Alive3] >= 1 && TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += 990;
	}
	//����
	if (TypeCount[_ourSide][Sleep4] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep4] * 500;
	}
	if (TypeCount[_counterSide][Sleep4] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep4] * 490;
	}
	//����
	if (TypeCount[_ourSide][Alive3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive3] * 200;
	}
	if (TypeCount[_counterSide][Alive3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive3] * 190;
	}
	//˫���
	if (TypeCount[_ourSide][Alive2] >= 2)
	{
		_ourScore += 100;
	}
	if (TypeCount[_counterSide][Alive2] >= 2)
	{
		_counterScore += 90;
	}
	//����
	if (TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep3] * 50;
	}
	if (TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep3] * 40;
	}
	//����߶�
	if (TypeCount[_ourSide][Alive2] >= 1 && TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += 10;
	}
	if (TypeCount[_counterSide][Alive2] >= 1 && TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += 10;
	}
	//���
	if (TypeCount[_ourSide][Alive2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive2] * 5;
	}
	if (TypeCount[_counterSide][Alive2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive2] * 5;
	}
	//�߶�
	if (TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep2] * 3;
	}
	if (TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep2] * 3;
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
					_ourScore += PosValue[i*GRID_NUM+j];
				else
					_counterScore += PosValue[i*GRID_NUM + j];
			}

		}
	}
	//qDebug() << "_ourScore - _counterScore" << _ourScore - _counterScore;
	return _ourScore - _counterScore;

	//=============================================================================
	//return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
}
/*
 * ��һ����
 */
void PositionStruct::MakeMove(int pos)
{
	AddPiece(pos, sdPlayer);
	nDistance++;
	ChangeSide();
}
/*
 * ������һ����
 */
void PositionStruct::UndoMakeMove(int pos)
{ 
	DelPiece(pos, sdPlayer);
	nDistance--;
	ChangeSide();
}
/*
 * ���������߷�,�����п��е����ӵ����mvs����,���ؿ��н����
 */
int PositionStruct::GenerateMoves(int* mvs) 
{
	/*
	 * ֻ���������ӷ�Χ��3��������
	 */
	int j = 0;
	for (int i=0;i<GRID_COUNT;i++)
	{
		if (ucpcSquares[i] == -1)//����һ���ո�
		{
			if(NeighborSum(i)>-25)//���������Ϊ���ĵ�5��5��Χ�����κ�һ��������
			{
				mvs[j] = i;
				j++;				
			}

		}
	}
	return j;
}
/*
 * �ж��߷��Ƿ����
 */
bool PositionStruct::LegalMove(int pos) const
{
	if (ucpcSquares[pos]!=-1)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}
/*
 *��ĳ���ӵ�5*5�����
 */
int PositionStruct::NeighborSum(int pos)
{
	int sum = 0;
	int y = pos / GRID_NUM;
	int x = pos - y*GRID_NUM;

	int X[5];
	int Y[5];
	int n = 0;
	for (int i=-2;i<3;i++)
	{
		X[n] = x + i;
		Y[n] = y + i;
		n++;
	}
	for (int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			if(X[i]>=0&&Y[j]>=0 && X[i] <= 14 && Y[j] <= 14)
			{
				sum += ucpcSquares[Y[j] * 15 + X[i]];
			}else
			{
				sum += -1;
			}
		}
	}
	return sum;
}

int PositionStruct::AnalysisLine(int * position, int GridNum, int StonePos)
{
	int StoneType;
	int AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;

	if (GridNum<5)
	{
		//���鳤��С��5û������
		memset(m_LineRecord, ANALSISED, sizeof(int)*GridNum);
		return 0;
	}
	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, sizeof(int) * 30);
	memset(AnalyLine, 0, sizeof(int) * 30);
	//����������װ��AnalyLine;
	memcpy(&AnalyLine, position, GridNum*sizeof(int));
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

int PositionStruct::AnalysisMainDiagonal(int position[][GRID_NUM], int i, int j)
{
	int tempArray[GRID_NUM];
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

int PositionStruct::AnalysisVertical(int position[][GRID_NUM], int i, int j)
{
	int tempArray[GRID_NUM];
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

int PositionStruct::Analysis_horizon(int position[][15], int i, int j)
{
	//����ֱ�߷�����������
	AnalysisLine(position[i], 15, j);
	//ʰȡ�������
	for (int s = 0; s<15; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];

	return TypeRecord[i][j][0];
}

int PositionStruct::AnalysisSubDiagonal(int position[][15], int i, int j)
{
	int tempArray[GRID_NUM];
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
