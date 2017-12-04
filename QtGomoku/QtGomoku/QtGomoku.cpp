#include "QtGomoku.h"
#include <QGraphicsItem>
#include <QPointF>
#include <csignal>
#include <qDebug>
#include <QMessageBox>
#include <QFile>


/*
 * ����ͳ�ʼ��
 */
QtGomoku::QtGomoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);

	WhiteStone = new QPixmap(":/QtGomoku/Resources/white.png");
	BlackStone = new QPixmap(":/QtGomoku/Resources/black.png");
	for(int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			BoardMap[i][j] = space;
		}
	}
	//m_ai = new AiAgent();
	//connect(this, SIGNAL(IsTimeForAiToCalculate(POINT*)), m_ai, SLOT(GetAiAction(POINT*)));
	//connect(m_ai, SIGNAL(AIComplete(POINT*)), this, SLOT(OnAiComplete(POINT*)));
	robot = new Robot();
	connect(this, SIGNAL(IsTimeForAiToCalculate(POINT*)), robot, SLOT(getAiResponse(POINT*)), Qt::QueuedConnection);
	connect(robot, SIGNAL(AIComplete(POINT*)), this, SLOT(OnAiComplete(POINT*)), Qt::QueuedConnection);
	
}
/*
 * ����������Ͽ��ͷ�
 */
void QtGomoku::OnMouseReleaseOnBoardSense(QPoint* pos)
{
	if (CurrentTurn == Human)
	{
		int qx = pos->x() / 40;
		int qy = pos->y() / 40;

		PutDownStone(qx, qy);//�����������
		POINT * _humanPos = new  POINT;
		_humanPos->x = qx;
		_humanPos->y = qy;
		emit IsTimeForAiToCalculate(_humanPos);
		qDebug() << "emit IsTimeForAiToCalculate, human pos:("<< qx << ","<<qy<<")";
		ui.statusBar->showMessage(QStringLiteral("AI���ڼ���..."));
	}
}
/*
 * ���ݵ�ǰ�������ˢ����������
 */
void QtGomoku::UpdateWholeBoardView()
{
	m_scene->clear();
	for (int i=0;i<GRID_NUM;i++)
	{
		for(int j=0;j<GRID_NUM;j++)
		{
			QGraphicsPixmapItem* Black = new QGraphicsPixmapItem(*BlackStone);
			QGraphicsPixmapItem* White = new QGraphicsPixmapItem(*WhiteStone);
			switch(BoardMap[i][j])
			{
			case white:
				White->setPos(QPoint(i * 40, j * 40));
				m_scene->addItem(White);
				break;
			case black:
				Black->setPos(QPoint(i * 40, j * 40));
				m_scene->addItem(Black);
				break;
			case space:
				break;
			default:
				break;
			}
		}
	}
}
/*
 * ��(x,y)��(x��y��[0,14])����ָ��һ��������,��������Ȩ
 */
void QtGomoku::PutDownStone(int x, int y)
{
	if(x<0||y<0||x>14||y>>14)
	{
		QMessageBox::information(this, QStringLiteral("����!"), QStringLiteral("��������Ƿ�!"));
		return;
	}
	if (BoardMap[x][y]!=space)
	{
		QMessageBox::information(this, QStringLiteral("����!"), QStringLiteral("���������Ѿ�����������!"));
		return;
	}
	if(BoardMap[x][y]==space)//���λ���ǿյ�
	{
		QGraphicsPixmapItem *qpi;
		if(CurrentTurn == Human)//�����������
		{
			BoardMap[x][y] = HumanColor;
			qpi = new QGraphicsPixmapItem((HumanColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Ai;
			QString temp = QStringLiteral("���:  x:");temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
		}
		else
		{
			BoardMap[x][y] = AiColor;
			qpi = new QGraphicsPixmapItem((AiColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Human;
			QString temp = QStringLiteral("AI:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
		}
		ui.listWidget_History->scrollToBottom();
		qpi->setPos(QPoint(x * 40, y * 40));
		m_scene->addItem(qpi);
		if (isGameOver(x, y) == true)
		{
			if(BoardMap[x][y] == AiColor)
			{
				//AIʤ��
				QMessageBox::information(this, QStringLiteral("���ź�"), QStringLiteral("AI�Ѿ�ȡʤ!"));
				//��Ϸ����
			}
			else
			{
				//���ʤ��
				QMessageBox::information(this, QStringLiteral("��ϲ"), QStringLiteral("�������AI!"));
				//��Ϸ���� 
			}
		}
	}
}
/*
 * �ж���Ϸ�Ƿ���Ϊ�ո����µ�������Ӷ�����
 */
bool QtGomoku::isGameOver(int x, int y)
{
	//����������Ӻ���Ʋ���ĸ�����,����������Ӯ
	Color _color= BoardMap[x][y];//��ǰ���ӵ���ɫ
	int vCount = 0, hCount = 0, lCount = 0, rCount = 0;
	for(int i=0;i<GRID_NUM;i++)//ˮƽ����ֱ
	{
		hCount = (BoardMap[i][y] == _color ? hCount + 1 : 0);
		vCount = (BoardMap[x][i] == _color ? vCount + 1 : 0);
		if(vCount >= 5 || hCount >= 5)
		{
			return true;//��Ϸ�Ѿ�����
		}

	}
	int m = GRID_NUM - abs(x - y);//���Խ��߷���Ԫ������
	int n = GRID_NUM - abs(GRID_NUM - (x + y + 1));//�ζԽ��߷���Ԫ������
	for (int i=-std::min(x,y);i<m-std::min(x,y);i++)
	{
		rCount =(BoardMap[x + i][y + i] == _color ? rCount + 1 : 0);//���Խ��߷���
		if(rCount >= 5)
		{
			return true;//��Ϸ�Ѿ�����
		}
	}
	int imax = n + (x - y) - 1 - std::min(x, GRID_NUM - 1 - y);
	int jmax = n - imax;
	for (int i = -imax; i<jmax; i++)
	{
		lCount = (BoardMap[x - i][y + i] == _color ? lCount + 1 : 0);//�ζԽ��߷���
		if (lCount >= 5)
		{
			return true;//��Ϸ�Ѿ�����
		}
	}
	return false;//��Ϸ��û�н���
}
/*
 * ��ӦAI�������
 */
void QtGomoku::OnAiComplete(POINT * pos)
{
	qDebug() << "OnAiComplete is running...";
	PutDownStone(pos->x, pos->y);
	ui.statusBar->clearMessage();
	ui.statusBar->showMessage(QStringLiteral("AI�������!"));
}
