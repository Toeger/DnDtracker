#include "character_widget.h"
#include "character_selector_widget.h"
#include "ui_character_widget.h"

#include <QCloseEvent>
#include <memory>
#include <string>
#include <tuple>

static Character_widget::Table_column table_columns[] = {
	{"Name", [](const Character &character) { return character.name; }},
	{"AC", [](const Character &character) { return QString::number(character.AC); }},
	{"Initiative", [](const Character &character) { return QString::number(character.initiative); }},
	{"HP",
	 [](const Character &character) {
		 return QString::number(character.current_HP) + (character.temporary_HP != 0 ? "+" + QString::number(character.temporary_HP) : "") + "/" +
				QString::number(character.max_HP);
	 }},
	{"Movement", [](const Character &character) { return QString::number(character.speed); }},
	{"Reaction",
	 [](const Character &character) {
		 (void)character;
		 return QString{};
	 }},
	{"Concentration",
	 [](const Character &character) {
		 (void)character;
		 return QString{};
	 }},
	{"Spell Slots",
	 [](const Character &character) {
		 (void)character;
		 return QString{};
	 }},
	{"Spell DC",
	 [](const Character &character) {
		 (void)character;
		 return QString{};
	 }},
	{"Passive Perception", [](const Character &character) { return QString{character.passive_perception}; }},
	{"Stealth",
	 [](const Character &character) {
		 (void)character;
		 return QString{};
	 }},
};

Character_widget::Character_widget(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::Character_widget} {
	ui->setupUi(this);
	columns.insert(std::begin(columns), std::begin(table_columns), std::end(table_columns));
	ui->character_table->setSortingEnabled(false);
}

Character_widget::~Character_widget() {
	delete ui;
}

void Character_widget::add_characters(std::vector<Character> &new_characters) {
	characters.insert(std::end(characters), std::make_move_iterator(std::begin(new_characters)), std::make_move_iterator(std::end(new_characters)));
	update_character_data();
}

void Character_widget::update_character_data() {
	ui->character_table->clear();
	ui->character_table->setColumnCount(columns.size());
	ui->character_table->setRowCount(0);
	for (int column = 0; static_cast<std::size_t>(column) < columns.size(); column++) {
		ui->character_table->setHorizontalHeaderItem(column, new QTableWidgetItem(columns[column].header));
	}
	for (int row = 0; static_cast<std::size_t>(row) < characters.size(); row++) {
		ui->character_table->insertRow(row);
		for (int column = 0; static_cast<std::size_t>(column) < columns.size(); column++) {
			auto item = std::make_unique<QTableWidgetItem>(columns[column].get_content(characters[row]));
			if (row == current_character) {
				item->setBackgroundColor(QColor::fromRgb(154, 189, 220));
			}
			ui->character_table->setItem(row, column, item.release());
		}
	}
}

void Character_widget::on_test_button_clicked() {
	static int character_counter{};
	characters.push_back({"Test name" + QString::number(character_counter++)});
	update_character_data();
}

void Character_widget::on_next_character_button_clicked() {
	current_character++;
	if (static_cast<std::size_t>(current_character) >= characters.size()) {
		current_character = 0;
	}
	update_character_data();
}

void Character_widget::on_roll_initiative_button_clicked() {
	for (auto &character : characters) {
		character.roll_initiative();
	}
	std::sort(std::begin(characters), std::end(characters), [](const Character &lhs, const Character &rhs) {
		return std::tie(lhs.initiative, lhs.initiative_modifier) > std::tie(rhs.initiative, rhs.initiative_modifier);
	});
	update_character_data();
}

void Character_widget::on_add_character_button_clicked() {
	character_selector_widget.reset();
	character_selector_widget = std::make_unique<Character_selector>(nullptr, this);
}