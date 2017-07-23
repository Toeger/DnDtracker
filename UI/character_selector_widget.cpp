#include "character_selector_widget.h"
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

const auto auto_save_filepath = [] {
	const auto path = QDir::homePath() + "/.DnDtracker";
	if (QDir{}.mkpath(path) == false) {
		return QString{};
	}
	return path + "/autosave.json";
}();

Character_selector::Character_selector(QWidget *parent, Character_widget *target)
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

	for (auto spinbox : {ui->str_spinbox, ui->dex_spinbox, ui->con_spinbox, ui->int_spinbox, ui->wis_spinbox, ui->cha_spinbox}) {
		connect(spinbox, qOverload<int>(&QSpinBox::valueChanged), this, &Character_selector::stat_spinbox_changed);
	}
}

Character_selector::~Character_selector() {
	to_json(auto_save_filepath);
	delete ui;
}

void Character_selector::on_add_to_character_list_clicked() {
	if (ui->name_edit->text().isEmpty()) {
		QMessageBox::information(this, tr("Unnamed character"), tr("Please enter a name for the new character before adding it to the list."));
		return;
	}
	characters.push_back(from_ui());
	update_character_list();
}

void Character_selector::update_character_list() {
	ui->character_list->clear();
	for (const auto &cha : characters) {
		ui->character_list->addItem(cha.name);
	}
}

Character Character_selector::from_ui() const {
	Character cha;
	cha.AC = ui->ac_spinbox->value();
	cha.charisma = ui->cha_spinbox->value();
	cha.constitution = ui->con_spinbox->value();
	cha.current_HP = ui->hp_spinbox->value();
	cha.dexterity = ui->dex_spinbox->value();
	cha.initiative_modifier = ui->initmod_spinbox->value();
	cha.intelligence = ui->int_spinbox->value();
	cha.max_HP = ui->hp_spinbox->value();
	cha.name = ui->name_edit->text();
	cha.passive_perception = ui->pp_spinbox->value();
	cha.proficiency = ui->proficiency_spinbox->value();
	cha.speed = ui->speed_spinbox->value();
	cha.strength = ui->str_spinbox->value();
	cha.wisdom = ui->wis_spinbox->value();
	cha.size = ui->size_selector->currentText();
	cha.type = ui->type_selector->currentText();
	cha.tag = ui->tag_selector->currentText();
	cha.alignment = ui->alignment_selector->currentText();
	cha.challenge_rating = ui->cr_selector->currentText();
	cha.armor = ui->armor_selector->currentText();
	cha.experience = ui->exp_selector->currentText().toInt();
	return cha;
}

void Character_selector::to_ui(const Character &cha) {
	ui->ac_spinbox->setValue(cha.AC);
	ui->cha_spinbox->setValue(cha.charisma);
	ui->con_spinbox->setValue(cha.constitution);
	ui->dex_spinbox->setValue(cha.dexterity);
	ui->initmod_spinbox->setValue(cha.initiative_modifier);
	ui->int_spinbox->setValue(cha.intelligence);
	ui->hp_spinbox->setValue(cha.max_HP);
	ui->name_edit->setText(cha.name);
	ui->pp_spinbox->setValue(cha.passive_perception);
	ui->proficiency_spinbox->setValue(cha.proficiency);
	ui->speed_spinbox->setValue(cha.speed);
	ui->str_spinbox->setValue(cha.strength);
	ui->wis_spinbox->setValue(cha.wisdom);
	ui->size_selector->setCurrentText(cha.size);
	ui->type_selector->setCurrentText(cha.type);
	ui->tag_selector->setCurrentText(cha.tag);
	ui->alignment_selector->setCurrentText(cha.alignment);
	ui->cr_selector->setCurrentText(cha.challenge_rating);
	ui->armor_selector->setCurrentText(cha.armor);
	ui->exp_selector->setEditText(QString::number(cha.experience));
};

void Character_selector::to_json(const QString &path) {
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

void Character_selector::from_json(const QString &path) {
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

void Character_selector::on_character_list_currentRowChanged(int currentRow) {
	if (currentRow != -1) {
		to_ui(characters[currentRow]);
	}
}

void Character_selector::on_add_selected_characters_button_clicked() {
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

void Character_selector::stat_spinbox_changed(int value) {
	struct Spinbox_data {
		QSpinBox *spinbox;
		QLineEdit *label;
		QString text;
	} spinbox_data[] = {
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

void Character_selector::on_save_character_changes_button_clicked() {
	characters[ui->character_list->currentRow()] = from_ui();
}
