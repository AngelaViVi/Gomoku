#pragma once
#include "StaticDefinition.h"

/*
 * �û���
 * �����ڦ�-�����������ж�״̬����ѹ��
 * ѹ�����״̬��һ��ɢ��ֵ����ʽ����,�������Ժ���������ܹ����ٷ������״̬�Ƿ����������۹�
 * �����������ǰ���۹��ķ���,��ֱ��ȡ��ԭ��������ֵ
 */
class TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();
private:
	unsigned int m_nHashKey32[15][10][9];  //32λ������飬��������32λ��ϣֵ
	unsigned int m_HashKey32;			   //32λ��ϣֵ
	unsigned __int64 m_ulHashKey64[15][10][9];//64λ������飬��������64λ��ϣֵ
	__int64 m_HashKey64;          //64 λ��ϣֵ
	HashItem *m_pTT[10];          //�û���ͷָ��
private:
	__int64 rand64();
	long rand32();
public:
	void InitializeHashKey();
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo);
	int LookUpHashTable(int alpha, int beta, int depth, int TableNo);
	void Hash_UnMakeMove(STONEMOVE *move, unsigned char CurPosition[][GRID_NUM]);
	void Hash_MakeMove(STONEMOVE *move, unsigned char CurPosition[][GRID_NUM]);
	void CalculateInitHashKey(unsigned char CurPosition[][GRID_NUM]);
	void _CTranspositionTable();
};