#pragma once

/*
 * ���峣��,�ṹ��,ö�ٵ�
 */
enum Color
{
	space = -1,/*������*/
	black = 0, /*����*/
	white = 1  /*����*/
};

enum Player
{
	Human = 0,
	Ai = 1
};

enum ENTRY_TYPE
{
	exact,			/*��ȷ*/
	lower_bound,	/*�½�*/
	upper_bound		/*�Ͻ�*/
};


//��ϣ����Ԫ�صĽṹ����
typedef struct HASHITEM
{
	__int64 checksum;         //64λУ����
	ENTRY_TYPE entry_type;	  //��������
	short depth;			  //ȡ�ô�ֵʱ�Ĳ��
	short eval;				  //�ڵ��ֵ
}HashItem;

typedef struct Node
{
	int x;
	int y;
}POINT;

//���Ա�ʾ����λ�õĽṹ
typedef struct _stoneposition
{
	unsigned char x;
	unsigned char y;
}STONEPOS;

typedef struct _movestone
{
	unsigned char nRenjuID;
	POINT ptMovePoint;
}MOVESTONE;

//����ṹ���Ա�ʾ�߷�

typedef struct _stonemove
{
	STONEPOS StonePos;//����λ��
	int Score;         //�߷��ķ���
}STONEMOVE;

#define GRID_NUM    15 //ÿһ��(��)�����̽�����
#define GRID_COUNT  225//�����Ͻ�������
/*����*/
#define TOBEANALSIS 0  //��δ����,����������
#define STWO        1  //�߶�
#define STHREE      2  //����
#define SFOUR       3  //����
#define TWO         4  //���
#define THREE       5  //����
#define FOUR        6  //����
#define FIVE        7  //����
#define NOTYPE      11 //δ����
#define ANALSISED   255//�ѷ�������