#ifndef CHARACTER_SELECTOR_H
#define CHARACTER_SELECTOR_H

#include "Logic/character.h"

#include <QWidget>

namespace Ui {
	class Character_selector;
}

class Character_selector : public QWidget {
	Q_OBJECT

	public:
	explicit Character_selector(QWidget *parent = 0);
	~Character_selector();
	void closeEvent(QCloseEvent *event) override;

	private slots:
	void on_add_to_character_list_clicked();
	void on_character_list_currentRowChanged(int currentRow);

	private:
	Ui::Character_selector *ui;
	std::vector<Character> characters;
	void update_character_list();
	Character from_ui() const;
	void to_ui(const Character &cha);
	void to_json(const QString &path);
};

#endif // CHARACTER_SELECTOR_H
