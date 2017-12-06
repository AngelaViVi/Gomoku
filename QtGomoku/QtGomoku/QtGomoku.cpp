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
	setFixedSize(this->width(), this->height());  //��ֹ������С
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);

	WhiteStone = new QPixmap(":/QtGomoku/Resources/white.png");
	BlackStone = new QPixmap(":/QtGomoku/Resources/black.png");
	MarkForLastStep = new QPixmap(":/QtGomoku/Resources/kh.png");
	Mark= new QGraphicsPixmapItem(*MarkForLastStep);
	for(int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			BoardMap[i][j] = space;
		}
	}
	robot = new AlphaBetaRobot();
	connect(this, SIGNAL(IsTimeForAiToCalculate(POINT*)), robot, SLOT(getAiResponse(POINT*)), Qt::QueuedConnection);
	connect(robot, SIGNAL(AIComplete(POINT*)), this, SLOT(OnAiComplete(POINT*)), Qt::QueuedConnection);
	Count = 1;
	ui.radioButton_PVE->setChecked(true);
	ui.radioButton_HumanBlack->setChecked(true);
	ui.radioButton_UnableForbidden->setChecked(true);

	ui.radioButton_PVP->setChecked(false);
	ui.radioButton_HumanWhite->setChecked(false);
	ui.radioButton_EnableForbidden->setChecked(false);
}
/*
 * ����������Ͽ��ͷ�(�������)
 */
void QtGomoku::OnMouseReleaseOnBoardSense(QPoint* pos)
{
	if (m_isGameOver==true)//��Ϸ�Ѿ�����,���ܼ�������
	{
		return;
	}
	if (isPVE == true)//Ŀǰ���˻���սģʽ
	{
		if (CurrentTurn == Human)
		{
			int qx = pos->x() / 40;
			int qy = pos->y() / 40;

			PutDownStone(qx, qy);//�����������
			POINT * _humanPos = new  POINT;
			_humanPos->x = qx;
			_humanPos->y = qy;
			if (isGameOver() == true)//��Ϸ���ܼ���,��֪ͨAI,���øú�����ֹ��Ҽ�������
			{
				//���øú���
				m_isGameOver = true;
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
	else//Ŀǰ�����˶�սģʽ
	{
		int qx = pos->x() / 40;
		int qy = pos->y() / 40;

		PutDownStone(qx, qy);//��������
		if (isGameOver() == true)//��Ϸ���ܼ���,��֪ͨAI,���øú�����ֹ��Ҽ�������
		{
			m_isGameOver = true;
			QMessageBox::information(this, QStringLiteral("��Ϸ����"), QStringLiteral("�������ж���Ӯ>_<"));
		}
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
	m_scene->removeItem(Mark);
	QGraphicsPixmapItem *qpi;
	QString temp = QString::number(Count, 10);  
	/*QGraphicsTextItem **/txtitem = new QGraphicsTextItem(temp);
	
	txtitem->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));
	if (isPVE == true)//�˻�ģʽ
	{
		if (CurrentTurn == Human)//�����������
		{
			BoardMap[x][y] = HumanColor;
			qpi = new QGraphicsPixmapItem((HumanColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Ai;
			QString temp = QStringLiteral("���:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
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
			txtitem->setDefaultTextColor((HumanColor == black ? QColor(0, 0, 0) : QColor(255, 255, 255)));
		}
	}
	else//����ģʽ
	{
		if (CurrentColor == black)//����
		{
			BoardMap[x][y] = black;
			qpi = new QGraphicsPixmapItem( *BlackStone );
			CurrentColor = white;
			QString temp = QStringLiteral("����:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor(QColor(255, 255, 255));
		}
		else//����
		{
			BoardMap[x][y] = white;
			qpi = new QGraphicsPixmapItem( *WhiteStone);
			CurrentColor = black;
			QString temp = QStringLiteral("����:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor( QColor(0, 0, 0));
		}
	}
	
	//������
	ui.listWidget_History->scrollToBottom();
	qpi->setPos(QPoint(x * 40, y * 40));
	m_scene->addItem(qpi);
	//д��
	int xaddin = (Count <= 9 && Count >= 0 ? 11 : (Count >= 10 && Count <= 99 ? 7.2 : 3.4));
	txtitem->setPos(QPoint(x * 40+ xaddin, y * 40+5.5));//����Ҫ���õĵ�λ��//x��ƫ������Ҫ����λ������
	m_scene->addItem(txtitem);//���item��scene��
	//��һ�����
	Mark->setPos(QPoint(x * 40, y * 40));
	m_scene->addItem(Mark);
	Count++;
}
/*
 * �ж���Ϸ�Ƿ����(����bug,ĳЩ����»�����)
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
	//qDebug() << "OnAiComplete is running...";
	PutDownStone(pos->x, pos->y);
	if (isGameOver() == true)//��Ϸ���ܼ���,��ֹ�������
	{
		//��Ϸ��ֹ
		m_isGameOver = true;
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
	//�������е���:��Ϸ�����в��ܵ�������
	ui.radioButton_PVE->setEnabled(false);
	ui.radioButton_PVP->setEnabled(false);
	ui.radioButton_EnableForbidden->setEnabled(false);
	ui.radioButton_UnableForbidden->setEnabled(false);
	ui.radioButton_HumanBlack->setEnabled(false);
	ui.radioButton_HumanWhite->setEnabled(false);
	ui.spinBox->setEnabled(false);
	//��ʼ��
	//�����������
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			BoardMap[i][j] = space;
		}
	}
	//�������
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);
	//AI��ʼ��
	robot = new AlphaBetaRobot();
	//�ӵ�һ����ʼ
	Count = 1;
	ui.listWidget_History->clear();
	//
	CurrentTurn = (isHumanGetBlack == true ? Human : Ai);
	CurrentColor = black;
	m_isGameOver = false;
	POINT * _SignalforAiFirst = new POINT();
	if (isPVE==true&&isHumanGetBlack==false)//�˻���ս���������ѡ��ִ�׺���
	{
		_SignalforAiFirst->x = -1;
		_SignalforAiFirst->y = -1;
		emit IsTimeForAiToCalculate(_SignalforAiFirst);
	}
}
/*
 * ����:����
 */
void QtGomoku::OnUndo()
{
	//�˻�ģʽ��,ֻ���ֵ��������������δ���ӵ�ʱ����ܻ���
	//1.֪ͨAI������һ���Լ������Ӻ���һ����ҵ�����
	//2.����������������һ��������
	//3.��putdown������ά��һ����������,��СΪ225,��¼˫��������˳��.
	//4.��������������,����ˢ�º���,�����������齫˫�������Ӱ�˳����
	//5.֪ͨAI��ʼ����,֪ͨ����Ϊ����������һ�����ӵ�λ��
	//����ģʽ��,��ʱ�����Ի���
	//1.�����������,������,��ʱ��Ϊ�ڷ����Ļ���,�����������һ�ֺͺ������һ��
	//2.�����������,������,��ʱ��Ϊ�׷����Ļ���,�����������һ�ֺͰ������һ��
	//3.Ȼ�����ˢ�º���������������ˢ��
	//���Լ�һ�����ܰ������������ļ�,Ȼ����븴�̹���,���ڼ�¼һЩ��Ϣ�Թ�����
}
/*
 * ����:������Ϸ
 */
void QtGomoku::OnStopGame()
{
	m_isGameOver = true;
	//�������е���:��Ϸ�����в��ܵ�������
	ui.radioButton_PVE->setEnabled(true);
	ui.radioButton_PVP->setEnabled(true);
	ui.radioButton_EnableForbidden->setEnabled(true);
	ui.radioButton_UnableForbidden->setEnabled(true);
	ui.radioButton_HumanBlack->setEnabled(true);
	ui.radioButton_HumanWhite->setEnabled(true);
	ui.spinBox->setEnabled(true);
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
	if(ui.radioButton_HumanBlack->isChecked()==false)
	{
		ui.radioButton_HumanBlack->setChecked(false);
		ui.radioButton_HumanWhite->setChecked(true);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("����") : QStringLiteral("���1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("���") : QStringLiteral("���2")));
		isHumanGetBlack = false;
		HumanColor = white;
		AiColor = black;
	}
	else
	{
		ui.radioButton_HumanBlack->setChecked(true);
		ui.radioButton_HumanWhite->setChecked(false);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("���") : QStringLiteral("���1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("����") : QStringLiteral("���2")));
		isHumanGetBlack = true;
		HumanColor = black;
		AiColor = white;
	}
}
/*
 * ��ѡ:���ִ��
 */
void QtGomoku::OnCheckedHumanWhite()
{
	if (ui.radioButton_HumanWhite->isChecked()==false)
	{
		ui.radioButton_HumanBlack->setChecked(true);
		ui.radioButton_HumanWhite->setChecked(false);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("���") : QStringLiteral("���1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("����") : QStringLiteral("���2")));
		isHumanGetBlack = true;
		HumanColor = black;
		AiColor = white;
	}
	else
	{
		ui.radioButton_HumanBlack->setChecked(false);
		ui.radioButton_HumanWhite->setChecked(true);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("����") : QStringLiteral("���1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("���") : QStringLiteral("���2")));
		isHumanGetBlack = false;
		HumanColor = white;
		AiColor = black;
	}
}
/*
 * ��ѡ:��������
 */
void QtGomoku::OnCheckedEnableForbidden()
{
	if (ui.radioButton_EnableForbidden->isChecked()==false)
	{
		ui.radioButton_EnableForbidden->setChecked(false);
		ui.radioButton_UnableForbidden->setChecked(true);
	}
	else
	{
		ui.radioButton_EnableForbidden->setChecked(true);
		ui.radioButton_UnableForbidden->setChecked(false);
	}
}
/*
 * ��ѡ:�رս���
 */
void QtGomoku::OnCheckedUnableForbidden()
{
	if (ui.radioButton_UnableForbidden->isChecked()==false)
	{
		ui.radioButton_EnableForbidden->setChecked(true);
		ui.radioButton_UnableForbidden->setChecked(false);
	}
	else
	{
		ui.radioButton_EnableForbidden->setChecked(false);
		ui.radioButton_UnableForbidden->setChecked(true);
	}
}
/*
 * ��ѡ:�˻���ս
 */
void QtGomoku::OnCheckedPVE()
{
	if (ui.radioButton_PVE->isChecked()== false)
	{
		ui.radioButton_PVE->setChecked(false);//����ѡ��
		ui.radioButton_PVP->setChecked(true);
		isPVE = false;
		ui.label_BlackOwner->setText(QStringLiteral("���1"));
		ui.label_WhiteOwner->setText(QStringLiteral("���2"));
		ui.radioButton_HumanBlack->setEnabled(false);
		ui.radioButton_HumanWhite->setEnabled(false);
	}else
	{
		ui.radioButton_PVE->setChecked(true);//����ѡ��
		ui.radioButton_PVP->setChecked(false);
		isPVE = true;
		ui.label_BlackOwner->setText((ui.radioButton_HumanBlack->isChecked()? QStringLiteral("���"): QStringLiteral("����")));
		ui.label_WhiteOwner->setText((ui.radioButton_HumanWhite->isChecked()? QStringLiteral("���") : QStringLiteral("����")));
		ui.radioButton_HumanBlack->setEnabled(true);
		ui.radioButton_HumanWhite->setEnabled(true);
	}
	
}
/*
 * ��ѡ:���˶�ս
 */
void QtGomoku::OnCheckedPVP()
{
	if (ui.radioButton_PVP->isChecked() == false)
	{
		ui.radioButton_PVP->setChecked(false);
		ui.radioButton_PVE->setChecked(true);//����ѡ��
		
		isPVE = true;
		ui.label_BlackOwner->setText((ui.radioButton_HumanBlack->isChecked() ? QStringLiteral("���") : QStringLiteral("����")));
		ui.label_WhiteOwner->setText((ui.radioButton_HumanWhite->isChecked() ? QStringLiteral("���") : QStringLiteral("����")));
		ui.radioButton_HumanBlack->setEnabled(true);
		ui.radioButton_HumanWhite->setEnabled(true);
	}
	else
	{
		ui.radioButton_PVP->setChecked(true);
		ui.radioButton_PVE->setChecked(false);//����ѡ��
		isPVE = false;
		ui.label_BlackOwner->setText(QStringLiteral("���1"));
		ui.label_WhiteOwner->setText(QStringLiteral("���2"));
		ui.radioButton_HumanBlack->setEnabled(false);
		ui.radioButton_HumanWhite->setEnabled(false);
	}
}
/*
 * Ai�ȼ��䶯
 */
void QtGomoku::OnAiLevelChanged(int newValue)
{
	switch (newValue)
	{
	case 1:
		robot->LIMIT_DEPTH = 4;
		break;
	case 2:
		robot->LIMIT_DEPTH = 8;
		break;
	case 3:
		robot->LIMIT_DEPTH = 16;
		break;
	case 4:
		robot->LIMIT_DEPTH = 32;
		break;
	case 5:
		robot->LIMIT_DEPTH = 64;
		break;
	default:
		robot->LIMIT_DEPTH = 32;
		ui.spinBox->setValue(4);
		break;
	}
}
