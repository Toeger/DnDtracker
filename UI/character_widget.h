#ifndef CHARACTER_WIDGET_H
#define CHARACTER_WIDGET_H

#include <QWidget>

namespace Ui {
	class Character_widget;
}

class Character_widget : public QWidget
{
	Q_OBJECT

	public:
	explicit Character_widget(QWidget *parent = 0);
	~Character_widget();

	private:
	Ui::Character_widget *ui;
};

#endif // CHARACTER_WIDGET_H
