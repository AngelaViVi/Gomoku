#include "QtGomoku.h"
#include <QGraphicsItem>
#include <QPointF>
#include <csignal>
#include <qDebug>
#include <QMessageBox>
#include <QFile>
#include <QString>


/*
 * ����ͳ�ʼ��
 */
QtGomoku::QtGomoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(this->width(), this->height());  //������á�
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
	Count = 1;
}
/*
 * ����������Ͽ��ͷ�(�������)
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
		if(isGameOver()==true)//��Ϸ���ܼ���,��֪ͨAI,���øú�����ֹ��Ҽ�������
		{
			CurrentTurn == GameOver;//���øú���
			//���ʤ��
			QMessageBox::information(this, QStringLiteral("��ϲ"), QStringLiteral("�������AI!"));
		}
		else//�����Ϸ���ܼ���,��֪ͨAI���м���
		{
			emit IsTimeForAiToCalculate(_humanPos);
			ui.statusBar->showMessage(QStringLiteral("AI���ڼ���..."));
		}
		//qDebug() << "emit IsTimeForAiToCalculate, human pos:("<< qx << ","<<qy<<")";
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
	//���λ���ǿյ�
	QGraphicsPixmapItem *qpi;
	QGraphicsTextItem *txtitem = new QGraphicsTextItem(QString::fromStdString(std::to_string(Count)));
	txtitem->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));
	if(CurrentTurn == Human)//�����������
	{
		BoardMap[x][y] = HumanColor;
		qpi = new QGraphicsPixmapItem((HumanColor == black ? *BlackStone : *WhiteStone));
		CurrentTurn = Ai;
		QString temp = QStringLiteral("���:  x:");temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
		ui.listWidget_History->addItem(temp);
		txtitem->setDefaultTextColor((HumanColor == black ? QColor(255, 255, 255) : QColor(0, 0, 0)));
	}
	else
	{
		BoardMap[x][y] = AiColor;
		qpi = new QGraphicsPixmapItem((AiColor == black ? *BlackStone : *WhiteStone));
		CurrentTurn = Human;
		QString temp = QStringLiteral("AI:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
		ui.listWidget_History->addItem(temp);
		txtitem->setDefaultTextColor((HumanColor == black ?  QColor(0, 0, 0): QColor(255, 255, 255)));
	}
	ui.listWidget_History->scrollToBottom();
	qpi->setPos(QPoint(x * 40, y * 40));
	m_scene->addItem(qpi);
	int xaddin = (Count <= 9 && Count >= 0 ? 11 : (Count >= 10 && Count <= 99 ? 7.2 : 3.4));
	txtitem->setPos(QPoint(x * 40+ xaddin, y * 40+5.5));//����Ҫ���õĵ�λ��//x��ƫ������Ҫ����λ������
	m_scene->addItem(txtitem);//���item��scene��
	Count++;
}
/*
 * �ж���Ϸ�Ƿ����
 */
bool QtGomoku::isGameOver()
{
	/*
	 * ȫ������,����û�����ӵĵ�
	 */
	//����������Ӻ���Ʋ���ĸ�����,����������Ӯ
	int vCount = 0, hCount = 0, lCount = 0, rCount = 0;
	for (int x=0;x<GRID_NUM;x++)
	{
		for (int y = 0; y < GRID_NUM; y++)
		{
			if(BoardMap[x][y]==space)
			{
				continue;
			}
			Color _color = BoardMap[x][y];//��ǰ���ӵ���ɫ
			for (int i = 0; i < GRID_NUM; i++)//ˮƽ����ֱ
			{
				hCount = (BoardMap[i][y] == _color ? hCount + 1 : 0);
				vCount = (BoardMap[x][i] == _color ? vCount + 1 : 0);
				if (vCount >= 5 || hCount >= 5)
				{
					return true;//��Ϸ�Ѿ�����
				}

			}
			int m = GRID_NUM - abs(x - y);//���Խ��߷���Ԫ������
			int n = GRID_NUM - abs(GRID_NUM - (x + y + 1));//�ζԽ��߷���Ԫ������
			for (int i = -std::min(x, y); i < m - std::min(x, y); i++)
			{
				rCount = (BoardMap[x + i][y + i] == _color ? rCount + 1 : 0);//���Խ��߷���
				if (rCount >= 5)
				{
					return true;//��Ϸ�Ѿ�����
				}
			}
			int imax = n + (x - y) - 1 - std::min(x, GRID_NUM - 1 - y);
			int jmax = n - imax;
			for (int i = -imax; i < jmax; i++)
			{
				lCount = (BoardMap[x - i][y + i] == _color ? lCount + 1 : 0);//�ζԽ��߷���
				if (lCount >= 5)
				{
					return true;//��Ϸ�Ѿ�����
				}
			}
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
	if (isGameOver() == true)//��Ϸ���ܼ���,��ֹ�������
	{
		CurrentTurn = GameOver;//��Ϸ��ֹ
		//AIʤ��
		QMessageBox::information(this, QStringLiteral("���ź�"), QStringLiteral("AI�Ѿ�ȡʤ!"));
	}		
	ui.statusBar->clearMessage();
	ui.statusBar->showMessage(QStringLiteral("AI�������!"));
}
/*
 * ����:����Ϸ
 */
void QtGomoku::OnNewGame()
{
}
/*
 * ����:����
 */
void QtGomoku::OnUndo()
{
}
/*
 * ����:������Ϸ
 */
void QtGomoku::OnStopGame()
{
}
/*
 * ����:����
 */
void QtGomoku::OnHelp()
{
}
/*
 * ��ѡ:���ִ��
 */
void QtGomoku::OnCheckedHumanBlack()
{
}
/*
 * ��ѡ:���ִ��
 */
void QtGomoku::OnCheckedHumanWhite()
{
}
/*
 * ��ѡ:��������
 */
void QtGomoku::OnCheckedEnableForbidden()
{
}
/*
 * ��ѡ:�رս���
 */
void QtGomoku::OnCheckedUnableForbidden()
{
}
/*
 * ��ѡ:��������
 */
void QtGomoku::OnCheckedSoundOn()
{
}
/*
 * ��ѡ:�ر�����
 */
void QtGomoku::OnCheckedSoundOff()
{
}
/*
 * ��ѡ:�˻���ս
 */
void QtGomoku::OnCheckedPVE()
{
}
/*
 * ��ѡ:���˶�ս
 */
void QtGomoku::OnCheckedPVP()
{
}
