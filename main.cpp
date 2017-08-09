#include "UI/mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QObject>

static QtMessageHandler default_message_handler{};
static MainWindow *mw{};

static void message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	switch (type) {
		case QtCriticalMsg:
		case QtFatalMsg: {
			const auto text = QObject::tr("\"%1\" caused by %2 in %3:%4").arg(msg, context.function, context.file, QString::number(context.line));
			qDebug() << "Error" << msg << text;
			QMessageBox::critical(mw, QObject::tr("Error"), text);
		} break;
		case QtWarningMsg: {
			const auto text = QObject::tr("\"%1\" caused by %2 in %3:%4").arg(msg, context.function, context.file, QString::number(context.line));
			qDebug() << "Warning" << msg << text;
			QMessageBox::warning(mw, QObject::tr("Warning"), text);
		} break;
		case QtDebugMsg:
		case QtInfoMsg:;
	}
	default_message_handler(type, context, msg);
}

int main(int argc, char *argv[]) {
	default_message_handler = qInstallMessageHandler(message_handler);
	QApplication a(argc, argv);
	MainWindow w;
	mw = &w;
	w.show();
	return a.exec();
}
