#ifndef CHARACTER_WIDGET_H
#define CHARACTER_WIDGET_H

#include <QString>
#include <QWidget>
#include <boost/variant.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "Logic/character.h"

namespace Ui {
	class Character_widget;
}

class Character_selector_widget;

class Character_widget : public QWidget {
	Q_OBJECT

	public:
	struct Table_column {
		using Table_column_type = boost::variant<QString, int, bool>;
		Table_column(QString header, QString Character::*string, std::function<void(Character &cha)> callback = [](Character &) {});
		Table_column(QString header, int Character::*number, std::function<void(Character &cha)> callback = [](Character &) {});
		Table_column(QString header, bool Character::*boolean, std::function<void(Character &cha)> callback = [](Character &) {});
		Table_column(QString header, std::function<std::unique_ptr<QWidget>(Character &)> get_widget);

		QString header{};
		std::function<std::unique_ptr<QWidget>(Character &)> get_widget{};
	};

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

	private:
	Ui::Character_widget *ui{};
	std::vector<Table_column> columns;
	int current_character{};
	std::unique_ptr<Character_selector_widget> character_selector_widget;
};

#endif // CHARACTER_WIDGET_H
