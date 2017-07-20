#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
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
