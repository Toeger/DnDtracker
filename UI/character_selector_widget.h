#ifndef CHARACTER_SELECTOR_H
#define CHARACTER_SELECTOR_H

#include "Logic/character.h"

#include <QWidget>

namespace Ui {
	class Character_selector;
}

class Character_widget;
class QSpinBox;

class Character_selector : public QWidget {
	Q_OBJECT

	public:
	explicit Character_selector(QWidget *parent = nullptr, Character_widget *target = nullptr);
	~Character_selector();

	private slots:
	void on_add_to_character_list_clicked();
	void on_character_list_currentRowChanged(int currentRow);
	void on_add_selected_characters_button_clicked();
	void stat_spinbox_changed(int value);
	void on_save_character_changes_button_clicked();

	private:
	Ui::Character_selector *ui;
	Character_widget *parent{};
	std::vector<Character> characters;
	void update_character_list();
	Character from_ui() const;
	void to_ui(const Character &cha);
	void to_json(const QString &path);
	void from_json(const QString &path);
};

#endif // CHARACTER_SELECTOR_H
