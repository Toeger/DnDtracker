#include "character_widget.h"
#include "Utility/assume.h"
#include "Utility/overloaded_function.h"
#include "Utility/unreachable.h"
#include "character_selector_widget.h"
#include "ui_character_widget.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <boost/variant.hpp>
#include <memory>
#include <string>
#include <tuple>

const static auto current_characters_filepath = "current_characters.json";

struct Table_column_data {
	QString header{};
	boost::variant<QString Character::*, int Character::*, bool Character::*> member_pointer{};
};

struct Table_column {
	Table_column(QString header, QString Character::*string)
		: header{std::move(header)}
		, get_widget([string](Character &character) {
			auto le = std::make_unique<QLineEdit>(character.*string);
			QObject::connect(le.get(), &QLineEdit::editingFinished, [&character, string, lineedit = le.get() ] {
				character.*string = lineedit->text();
				character_widget->update_character_data();
			});
			return le;
		}) {}

	Table_column(QString header, int Character::*number)
		: header{std::move(header)}
		, get_widget([number](Character &character) {
			auto box = std::make_unique<QSpinBox>();
			box->setValue(character.*number);
			QObject::connect(box.get(), &QSpinBox::editingFinished, [&character, number, spinbox = box.get() ] {
				character.*number = spinbox->value();
				character_widget->update_character_data();
			});
			return box;
		}) {}

	Table_column(QString header, bool Character::*boolean)
		: header{std::move(header)}
		, get_widget([boolean](Character &character) {
			auto box = std::make_unique<QCheckBox>();
			box->setCheckState(character.*boolean ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
			QObject::connect(box.get(), &QCheckBox::stateChanged, [&character, boolean](int state) {
				if (state == Qt::CheckState::Checked) {
					character.*boolean = true;
				} else if (state == Qt::CheckState::Unchecked) {
					character.*boolean = false;
				}
				character_widget->update_character_data();
			});
			return box;
		}) {}

	Table_column(QString header, std::function<std::unique_ptr<QWidget>(Character &)> get_widget)
		: header{std::move(header)}
		, get_widget{std::move(get_widget)} {}

	QString header{};
	std::function<std::unique_ptr<QWidget>(Character &)> get_widget{};
	static Character_widget *character_widget;
};
Character_widget *Table_column::character_widget{};

static const Table_column_data table_columns[] = {
	{"Name", &Character::name},
	{"AC", &Character::AC},
	{"Initiative", &Character::initiative},
	{"HP", &Character::current_HP},
	{"Movement", &Character::ground_speed},
	{"Reaction", &Character::reaction},
	{"Concentration", &Character::concentration},
	/*
	{"Spell Slots",
	 [](const Character &character) -> Table_column::Table_column_type {
		 (void)character;
		 return QString{};
	 }},*/
	/*
	{"Spell DC",
	 [](const Character &character) -> Table_column::Table_column_type {
		 (void)character;
		 return QString{};
	 }},*/
	{"Passive Perception", &Character::passive_perception},
	/*
	{"Stealth",
	 [](const Character &character) -> Table_column::Table_column_type {
		 (void)character;
		 return QString{};
	 }},*/
};

Character_widget::Character_widget(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::Character_widget} {
	Table_column::character_widget = this;
	ui->setupUi(this);
	ui->character_table->setSortingEnabled(false);
	columns.push_back({"X", [this](Character &cha) {
						   auto button = std::make_unique<QPushButton>();
						   button->setIcon(QIcon::fromTheme("window-close"));
						   QObject::connect(button.get(), &QPushButton::pressed, [this, &cha]() {
							   const auto it = std::find_if(std::begin(characters), std::end(characters),
															[&cha](const std::unique_ptr<Character> &ptr) { return &cha == ptr.get(); });
							   assume(it != std::end(characters));
							   characters.erase(it);
							   update_character_data();
						   });
						   return button;
					   }});
	std::transform(std::begin(table_columns), std::end(table_columns), std::back_inserter(columns), [](const Table_column_data &tcd) {
		return boost::apply_visitor(make_overloaded_function(
										[&header = tcd.header](int Character::*number) {
											return Table_column{header, number};
										},
										[&header = tcd.header](QString Character::*string) {
											return Table_column{header, string};
										},
										[&header = tcd.header](bool Character::*boolean) {
											return Table_column{header, boolean};
										}),
									tcd.member_pointer);
	});
	{
		QFile file{current_characters_filepath};
		if (file.open(QIODevice::ReadOnly)) {
			for (const auto &object : QJsonDocument::fromJson(file.readAll()).array()) {
				characters.push_back(std::make_unique<Character>(Character::from_json(object.toObject())));
			}
		}
	}
	update_character_data();
	ui->character_table->horizontalHeader()->resizeSections(QHeaderView::ResizeMode::ResizeToContents);
}

Character_widget::~Character_widget() {
	delete ui;
}

void Character_widget::add_characters(std::vector<Character> &new_characters) {
	std::transform(std::begin(new_characters), std::end(new_characters), std::back_inserter(characters),
				   [](Character &cha) { return std::make_unique<Character>(std::move(cha)); });
	update_character_data();
}

void Character_widget::update_character_data() {
	std::stable_sort(std::begin(characters), std::end(characters),
					 [](const std::unique_ptr<Character> &lhs, const std::unique_ptr<Character> &rhs) { return lhs->initiative > rhs->initiative; });
	{
		QJsonArray array;
		for (const auto &character : characters) {
			array += character->to_json();
		}
		QFile file{current_characters_filepath};
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		file.write(QJsonDocument{std::move(array)}.toJson());
	}

	ui->character_table->clear();
	ui->character_table->setColumnCount(columns.size());
	ui->character_table->setRowCount(0);

	for (int column = 0; static_cast<std::size_t>(column) < columns.size(); column++) {
		ui->character_table->setHorizontalHeaderItem(column, new QTableWidgetItem(columns[column].header));
	}

	for (int row = 0; static_cast<std::size_t>(row) < characters.size(); row++) {
		ui->character_table->insertRow(row);
		for (int column = 0; static_cast<std::size_t>(column) < columns.size(); column++) {
			auto item = columns[column].get_widget(*characters[row]);
			if (row == current_character_index && item != nullptr) {
				QPalette palette;
				palette.setColor(QPalette::Base, QColor::fromRgb(154, 189, 220));
				item->setPalette(palette);
			}
			ui->character_table->setCellWidget(row, column, item.release());
		}
	}
}

void Character_widget::on_next_character_button_clicked() {
	current_character_index++;
	if (static_cast<std::size_t>(current_character_index) >= characters.size()) {
		current_character_index = 0;
		ui->round_count_spinbox->setValue(ui->round_count_spinbox->value() + 1);
	}
	if (characters.empty() == false) {
		characters[current_character_index]->reaction = true;
	}
	update_character_data();
}

void Character_widget::on_roll_initiative_button_clicked() {
	for (auto &character : characters) {
		character->roll_initiative();
	}
	update_character_data();
	ui->round_count_spinbox->setValue(1);
}

void Character_widget::on_add_character_button_clicked() {
	character_selector_widget.reset();
	character_selector_widget = std::make_unique<Character_selector_widget>(nullptr, this);
}

void Character_widget::on_refresh_button_clicked() {
	update_character_data();
}
