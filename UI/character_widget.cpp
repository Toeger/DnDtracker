#include "character_widget.h"
#include "Utility/overloaded_function.h"
#include "character_selector_widget.h"
#include "ui_character_widget.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QLineEdit>
#include <QSpinBox>
#include <memory>
#include <string>
#include <tuple>

static Character_widget::Table_column table_columns[] = {
	{"Name", [](const Character &character) -> Character_widget::Table_column::Table_column_type { return character.name; }},
	{"AC", [](const Character &character) -> Character_widget::Table_column::Table_column_type { return character.AC; }},
	{"Initiative", [](const Character &character) -> Character_widget::Table_column::Table_column_type { return character.initiative; }},
	{"HP",
	 [](const Character &character) -> Character_widget::Table_column::Table_column_type {
		 const auto starting_hp = character.get_max_hp();
		 return QString::number(starting_hp) + "/" + QString::number(starting_hp);
	 }},
	{"Movement", [](const Character &character) -> Character_widget::Table_column::Table_column_type { return character.speed; }},
	{"Reaction", [](const Character &) -> Character_widget::Table_column::Table_column_type { return true; }},
	{"Concentration", [](const Character &) -> Character_widget::Table_column::Table_column_type { return false; }},
	{"Spell Slots",
	 [](const Character &character) -> Character_widget::Table_column::Table_column_type {
		 (void)character;
		 return QString{};
	 }},
	{"Spell DC",
	 [](const Character &character) -> Character_widget::Table_column::Table_column_type {
		 (void)character;
		 return QString{};
	 }},
	{"Passive Perception", [](const Character &character) -> Character_widget::Table_column::Table_column_type { return character.passive_perception; }},
	{"Stealth",
	 [](const Character &character) -> Character_widget::Table_column::Table_column_type {
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

static auto get_table_widget(const Character_widget::Table_column::Table_column_type &data) {
	auto make_line_edit = [](const QString &string) -> std::unique_ptr<QWidget> { return std::make_unique<QLineEdit>(string); };
	auto make_checkbox = [](bool b) -> std::unique_ptr<QWidget> {
		auto box = std::make_unique<QCheckBox>();
		box->setCheckState(b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		return box;
	};
	auto make_spinbox = [](int i) -> std::unique_ptr<QWidget> {
		auto box = std::make_unique<QSpinBox>();
		box->setValue(i);
		return box;
	};

	return boost::apply_visitor(make_overloaded_function(make_line_edit, make_spinbox, make_checkbox), data);
};

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
			auto item = get_table_widget(columns[column].get_content(characters[row]));
			if (row == current_character && item != nullptr) {
				QPalette palette;
				palette.setColor(QPalette::Base, QColor::fromRgb(154, 189, 220));
				item->setPalette(palette);
			}
			ui->character_table->setCellWidget(row, column, item.release());
		}
	}

	ui->character_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
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
	character_selector_widget = std::make_unique<Character_selector_widget>(nullptr, this);
}