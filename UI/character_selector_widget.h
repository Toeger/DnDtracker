#ifndef CHARACTER_SELECTOR_H
#define CHARACTER_SELECTOR_H

#include "Logic/character.h"

#include <QWidget>
#include <memory>
#include <vector>

namespace Ui {
	class Character_selector;
}

class Character_widget;
class Character_skills_widget;
class QSpinBox;
struct Member_ui;

class Character_selector_widget : public QWidget {
	Q_OBJECT

	public:
	explicit Character_selector_widget(QWidget *parent = nullptr, Character_widget *target = nullptr);
	~Character_selector_widget();

	private slots:
	void on_add_to_character_list_clicked();
	void on_character_list_currentRowChanged(int currentRow);
	void on_add_selected_characters_button_clicked();
	void stat_spinbox_changed(int value);
	void on_save_character_changes_button_clicked();
	void on_fill_stats_based_on_challenge_rating_button_clicked();
	void recalculate_hp();
	void on_skills_edit_button_clicked();

	private:
	Ui::Character_selector *ui{};
	Character_widget *parent{};
	std::vector<Character> characters{};
	std::unique_ptr<Character_skills_widget> character_skills_widget{};

	void update_character_list();
	Character from_ui() const;
	void to_ui(const Character &cha);
	void to_json(const QString &path);
	void from_json(const QString &path);
	std::vector<Member_ui> get_member_ui_binding() const;
};

#endif // CHARACTER_SELECTOR_H
