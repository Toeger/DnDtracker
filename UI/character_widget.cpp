#include "character_widget.h"
#include "ui_character_widget.h"

Character_widget::Character_widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Character_widget)
{
	ui->setupUi(this);
}

Character_widget::~Character_widget()
{
	delete ui;
}
