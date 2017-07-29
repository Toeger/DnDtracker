#ifndef CHARACTER_SKILLS_WIDGET_H
#define CHARACTER_SKILLS_WIDGET_H

#include <QWidget>

namespace Ui {
	class Character_skills_widget;
}

class Character_selector_widget;

class Character_skills_widget : public QWidget {
	Q_OBJECT

	public:
	explicit Character_skills_widget(QWidget *parent = 0, Character_selector_widget *target = nullptr);
	~Character_skills_widget();

	private:
	Ui::Character_skills_widget *ui{};
	Character_selector_widget *target{};
};

#endif // CHARACTER_SKILLS_WIDGET_H
