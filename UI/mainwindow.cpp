#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	const auto path = QDir::homePath() + "/.DnDtracker";
	QDir{}.mkpath(path);
	QDir::setCurrent(path);
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	delete ui->characters_widget;
	ui->characters_widget = nullptr;
	delete ui;
	ui = nullptr;
	event->accept();
}
