#include "character_selector_widget.h"
#include "Utility/overloaded_function.h"
#include "character_skills_widget.h"
#include "character_widget.h"
#include "ui_character_selector.h"
#include "ui_utility.h"

#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSpinBox>
#include <QtGlobal>
#include <boost/variant.hpp>

struct Member_ui {
	boost::variant<int Character::*, QString Character::*> member{};
	boost::variant<QSpinBox *, QLineEdit *, QComboBox *> ui_element{};
};

const auto auto_save_filepath = [] {
	const auto path = QDir::homePath() + "/.DnDtracker";
	if (QDir{}.mkpath(path) == false) {
		return QString{};
	}
	return path + "/autosave.json";
}();

Character_selector_widget::Character_selector_widget(QWidget *parent, Character_widget *target)
	: QWidget(parent)
	, ui(new Ui::Character_selector)
	, parent(target) {
	ui->setupUi(this);
	setVisible(true);
	from_json(auto_save_filepath);
	update_character_list();
	ui->character_list->installEventFilter(new ui_utility::Event_filter(this, [this](QObject *, QEvent *event) {
		if (event->type() == QEvent::KeyPress) {
			auto key_event = static_cast<QKeyEvent *>(event);
			if (key_event->key() == Qt::Key::Key_Delete) {
				for (auto &item : ui->character_list->selectedItems()) {
					characters.erase(std::begin(characters) + ui->character_list->row(item));
					delete item;
				}
			}
		}
		return false;
	}));

	for (const auto &spinbox : {ui->str_spinbox, ui->dex_spinbox, ui->con_spinbox, ui->int_spinbox, ui->wis_spinbox, ui->cha_spinbox}) {
		connect(spinbox, qOverload<int>(&QSpinBox::valueChanged), this, &Character_selector_widget::stat_spinbox_changed);
	}
}

Character_selector_widget::~Character_selector_widget() {
	to_json(auto_save_filepath);
	delete ui;
}

void Character_selector_widget::on_add_to_character_list_clicked() {
	if (ui->name_edit->text().isEmpty()) {
		QMessageBox::information(this, tr("Unnamed character"), tr("Please enter a name for the new character before adding it to the list."));
		return;
	}
	characters.push_back(from_ui());
	update_character_list();
}

void Character_selector_widget::update_character_list() {
	ui->character_list->clear();
	for (const auto &cha : characters) {
		ui->character_list->addItem(cha.name);
	}
}

Character Character_selector_widget::from_ui() const {
	Character cha;
	for (const auto &member_ui : get_member_ui_binding()) {
		boost::apply_visitor(
			make_overloaded_function(
				[&member_ui, &cha](int Character::*number) {
					boost::apply_visitor(make_overloaded_function([number, &cha](QSpinBox *spinbox) { cha.*number = spinbox->value(); },
																  [number, &cha](QLineEdit *lineedit) { cha.*number = lineedit->text().toInt(); },
																  [number, &cha](QComboBox *combobox) { cha.*number = combobox->currentText().toInt(); }),
										 member_ui.ui_element);
				},
				[&member_ui, &cha](QString Character::*string) {
					boost::apply_visitor(make_overloaded_function([string, &cha](QSpinBox *spinbox) { cha.*string = QString::number(spinbox->value()); },
																  [string, &cha](QLineEdit *lineedit) { cha.*string = lineedit->text(); },
																  [string, &cha](QComboBox *combobox) { cha.*string = combobox->currentText(); }),
										 member_ui.ui_element);
				}),
			member_ui.member);
	}
	cha.current_HP = cha.get_max_hp();
	return cha;
}

void Character_selector_widget::to_ui(const Character &cha) {
	for (const auto &member_ui : get_member_ui_binding()) {
		boost::apply_visitor(
			make_overloaded_function(
				[&member_ui, &cha](int Character::*number) {
					boost::apply_visitor(
						make_overloaded_function([number, &cha](QSpinBox *spinbox) { spinbox->setValue(cha.*number); },
												 [number, &cha](QLineEdit *lineedit) { lineedit->setText(QString::number(cha.*number)); },
												 [number, &cha](QComboBox *combobox) { combobox->setCurrentText(QString::number(cha.*number)); }),
						member_ui.ui_element);
				},
				[&member_ui, &cha](QString Character::*string) {
					boost::apply_visitor(make_overloaded_function([string, &cha](QSpinBox *spinbox) { spinbox->setValue((cha.*string).toInt()); },
																  [string, &cha](QLineEdit *lineedit) { lineedit->setText(cha.*string); },
																  [string, &cha](QComboBox *combobox) { combobox->setCurrentText(cha.*string); }),
										 member_ui.ui_element);
				}),
			member_ui.member);
	}
};

void Character_selector_widget::to_json(const QString &path) {
	QFile f{path};
	if (f.open(QIODevice::WriteOnly) == false) {
		QMessageBox::critical(this, tr("IO Error"), tr("Failed writing file\n%1\n").arg(path));
		return;
	}
	QJsonArray object;
	for (const auto &cha : characters) {
		object.append(cha.to_json());
	}
	QJsonDocument doc{object};
	f.write(doc.toJson());
}

void Character_selector_widget::from_json(const QString &path) {
	QFile f{path};
	if (f.open(QIODevice::ReadOnly) == false) {
		QMessageBox::critical(this, tr("IO Error"), tr("Failed reading file\n%1\n").arg(path));
		return;
	}
	const auto &data = f.readAll();
	QJsonParseError json_error;
	const auto &document = QJsonDocument::fromJson(data, &json_error);
	if (document.isNull()) {
		qDebug() << json_error.errorString();
	}
	const auto &object = document.array();
	for (const auto &cha : object) {
		characters.push_back(Character::from_json(cha.toObject()));
	}
}

std::vector<Member_ui> Character_selector_widget::get_member_ui_binding() const {
	return {
		{&Character::AC, ui->ac_spinbox},
		{&Character::charisma, ui->cha_spinbox},
		{&Character::constitution, ui->con_spinbox},
		{&Character::dexterity, ui->dex_spinbox},
		{&Character::initiative_modifier, ui->initmod_spinbox},
		{&Character::intelligence, ui->int_spinbox},
		{&Character::level, ui->level_selector},
		{&Character::hit_die, ui->dice_selector},
		{&Character::name, ui->name_edit},
		{&Character::passive_perception, ui->pp_spinbox},
		{&Character::proficiency, ui->proficiency_spinbox},
		{&Character::ground_speed, ui->ground_speed_spinbox},
		{&Character::swim_speed, ui->swim_speed_spinbox},
		{&Character::fly_speed, ui->fly_speed_spinbox},
		{&Character::other_speed, ui->other_speed_spinbox},
		{&Character::strength, ui->str_spinbox},
		{&Character::wisdom, ui->wis_spinbox},
		{&Character::size, ui->size_selector},
		{&Character::type, ui->type_selector},
		{&Character::tag, ui->tag_selector},
		{&Character::alignment, ui->alignment_selector},
		{&Character::challenge_rating, ui->cr_selector},
		{&Character::armor, ui->armor_selector},
		{&Character::experience, ui->exp_selector},
	};
}

void Character_selector_widget::on_character_list_currentRowChanged(int currentRow) {
	if (currentRow != -1) {
		to_ui(characters[currentRow]);
	}
}

void Character_selector_widget::on_add_selected_characters_button_clicked() {
	if (parent) {
		std::vector<Character> chars{};
		const auto &items = ui->character_list->selectedItems();
		chars.reserve(items.size());
		for (const auto &item : items) {
			chars.push_back(characters[ui->character_list->row(item)]);
		}
		parent->add_characters(chars);
	}
}

void Character_selector_widget::stat_spinbox_changed(int value) {
	struct Spinbox_data {
		QSpinBox *spinbox;
		QLineEdit *label;
		QString text;
	} const spinbox_data[] = {
		{ui->str_spinbox, ui->str_modifier_edit, tr("Str")}, {ui->dex_spinbox, ui->dex_modifier_edit, tr("Dex")},
		{ui->con_spinbox, ui->con_modifier_edit, tr("Con")}, {ui->int_spinbox, ui->int_modifier_edit, tr("Int")},
		{ui->wis_spinbox, ui->wis_modifier_edit, tr("Wis")}, {ui->cha_spinbox, ui->cha_modifier_edit, tr("Cha")},
	};

	auto get_modifier = [](const int value) {
		const int mod = value / 2 - 5;
		auto mod_string = QString::number(mod);
		if (mod >= 0) {
			mod_string.prepend('+');
		}
		return " (" + std::move(mod_string) + ')';
	};

	auto spinbox = dynamic_cast<QSpinBox *>(QObject::sender());
	for (auto &d : spinbox_data) {
		if (d.spinbox == spinbox) {
			d.label->setText(get_modifier(value));
			break;
		}
	}
}

void Character_selector_widget::on_save_character_changes_button_clicked() {
	characters[ui->character_list->currentRow()] = from_ui();
}

void Character_selector_widget::on_fill_stats_based_on_challenge_rating_button_clicked() {
	struct Cr_data {
		QString text{};
		int exp{};
		int proficiency{};
	} const cr_data[] = {
		{"0", 10, 2},      {"1/8", 25, 2},    {"1/4", 50, 2},     {"1/2", 100, 2},    {"1", 200, 2},      {"2", 450, 2},      {"3", 700, 2},
		{"4", 1'100, 2},   {"5", 1'800, 3},   {"6", 2'300, 3},    {"7", 2'900, 3},    {"8", 3'900, 3},    {"9", 5'000, 4},    {"10", 5'900, 4},
		{"11", 7'200, 4},  {"12", 8'400, 4},  {"13", 10'000, 5},  {"14", 11'500, 5},  {"15", 13'000, 5},  {"16", 15'000, 5},  {"17", 18'000, 6},
		{"18", 20'000, 6}, {"19", 22'000, 6}, {"20", 25'000, 6},  {"21", 33'000, 7},  {"22", 41'000, 7},  {"23", 50'000, 7},  {"24", 62'000, 7},
		{"25", 75'000, 8}, {"26", 90'000, 8}, {"27", 105'000, 8}, {"28", 120'000, 8}, {"29", 135'000, 9}, {"30", 155'000, 9},
	};

	const auto cr = ui->cr_selector->currentText();
	const auto pos = std::find_if(std::begin(cr_data), std::end(cr_data), [&cr](const Cr_data &rhs) { return cr == rhs.text; });
	if (pos != std::end(cr_data)) {
		ui->exp_selector->setEditText(QString::number(pos->exp));
		ui->proficiency_spinbox->setValue(pos->proficiency);
	}
}

void Character_selector_widget::recalculate_hp() {
	const auto &level = ui->level_selector->value();
	const auto &hit_die = ui->dice_selector->value();
	const auto &con = ui->con_spinbox->value();
	const auto &hp = Character::get_max_hp(level, hit_die, con);
	ui->hp_label->setText('(' + QString::number(hp) + ')');
}

void Character_selector_widget::on_skills_edit_button_clicked() {
	character_skills_widget = std::make_unique<Character_skills_widget>(nullptr, this);
}
