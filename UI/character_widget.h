#ifndef CHARACTER_WIDGET_H
#define CHARACTER_WIDGET_H

#include <QWidget>
#include <vector>
#include <QString>
#include <memory>

#include "Logic/character.h"

namespace Ui {
	class Character_widget;
}

class Character_selector;

class Character_widget : public QWidget {
	Q_OBJECT

	public:
	struct Table_column {
		QString header;
		QString (*get_content)(const Character &character);
	};

	std::vector<Character> characters;
	explicit Character_widget(QWidget *parent = 0);
	~Character_widget();

	public slots:
	void update_character_data();

	private slots:
	void on_next_character_button_clicked();
	void on_roll_initiative_button_clicked();
	void on_add_character_button_clicked();
	void on_test_button_clicked();

	private:
	Ui::Character_widget *ui{};
	std::vector<Table_column> columns;
	int current_character{};
	std::unique_ptr<Character_selector> character_selector_widget;
};

#endif // CHARACTER_WIDGET_H
