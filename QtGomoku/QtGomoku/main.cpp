#include "QtGomoku.h"
#include <QtWidgets/QApplication>
#include <qDebug>
#include <QFile>
#include <QDateTime>
#include <QMutex>
/*
 * ʵ����־����
 * 
 * ע�͵��Ĳ����ܼ�¼�������Ϣ��������ʱ
 */
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	// ����
	static QMutex mutex;
	mutex.lock();

	//QByteArray localMsg = msg.toLocal8Bit();

	//QString strMsg("");
	//switch (type)
	//{
	//case QtDebugMsg:
	//	strMsg = QString("Debug:");
	//	break;
	//case QtWarningMsg:
	//	strMsg = QString("Warning:");
	//	break;
	//case QtCriticalMsg:
	//	strMsg = QString("Critical:");
	//	break;
	//case QtFatalMsg:
	//	strMsg = QString("Fatal:");
	//	break;
	//}

	//// ���������Ϣ��ʽ
	//QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	//QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5")
	//	.arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

	//// �����Ϣ���ļ��У���д��׷����ʽ��
	//QFile file("Gomokulog.txt");
	//file.open(QIODevice::ReadWrite | QIODevice::Append);
	//QTextStream stream(&file);
	//stream << strMessage << "\r\n";
	//file.flush();
	//file.close();


	QString txtMessage;

	switch (type)
	{
	case QtDebugMsg:    //������Ϣ��ʾ
		txtMessage = QString("Debug: %1").arg(msg);
		break;

	case QtWarningMsg:    //һ���warning��ʾ
		txtMessage = QString("Warning: %1").arg(msg);
		break;

	case QtCriticalMsg:    //���ش�����ʾ
		txtMessage = QString("Critical: %1").arg(msg);
		break;

	case QtFatalMsg:    //����������ʾ
		txtMessage = QString("Fatal: %1").arg(msg);
		abort();
	}

	//������������Ϣ��ָ���ļ�
	QFile outputFile("Gomokulog.txt");
	outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream textStream(&outputFile);
	textStream << txtMessage << endl;
	// ����
	mutex.unlock();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qInstallMessageHandler(customMessageHandler);
	QtGomoku w;
	w.show();
	return a.exec();
}
