#ifndef CHARACTER_WIDGET_H
#define CHARACTER_WIDGET_H

#include <QString>
#include <QWidget>
#include <functional>
#include <memory>
#include <vector>

#include "Logic/character.h"

namespace Ui {
	class Character_widget;
}

class Character_selector_widget;
struct Table_column;

class Character_widget : public QWidget {
	Q_OBJECT
	public:
	std::vector<std::unique_ptr<Character>> characters;
	explicit Character_widget(QWidget *parent = 0);
	~Character_widget();
	void add_characters(std::vector<Character> &new_characters);

	public slots:
	void update_character_data();

	private slots:
	void on_next_character_button_clicked();
	void on_roll_initiative_button_clicked();
	void on_add_character_button_clicked();
	void on_refresh_button_clicked();

	private:
	Ui::Character_widget *ui{};
	std::vector<Table_column> columns;
	int current_character_index{};
	std::unique_ptr<Character_selector_widget> character_selector_widget{};
};

#endif // CHARACTER_WIDGET_H
