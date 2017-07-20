#include "character_selector_widget.h"
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

const auto auto_save_filepath = [] {
	const auto path = QDir::homePath() + "/.DnDtracker";
	if (QDir{}.mkpath(path) == false) {
		return QString{};
	}
	return path + "/autosave.json";
}();

Character_selector::Character_selector(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Character_selector) {
	ui->setupUi(this);
	setVisible(true);
	from_json(auto_save_filepath);
	update_character_list();
	ui->character_list->installEventFilter(new ui_utility::Event_filter(this, [this](QObject *, QEvent *event) {
		if (event->type() == QEvent::KeyPress) {
			auto key_event = static_cast<QKeyEvent *>(event);
			if (key_event->key() == Qt::Key::Key_Delete) {
				for (auto &item : ui->character_list->selectedItems()) {
					delete item;
				}
			}
		}
		return false;
	}));
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
		ui->character_list->addItem(QString::fromStdString(cha.name));
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
	cha.name = ui->name_edit->text().toStdString();
	cha.passive_perception = ui->pp_spinbox->value();
	cha.proficiency = ui->proficiency_spinbox->value();
	cha.speed = ui->speed_spinbox->value();
	cha.strength = ui->str_spinbox->value();
	cha.wisdom = ui->wis_spinbox->value();
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
	ui->name_edit->setText(QString::fromStdString(cha.name));
	ui->pp_spinbox->setValue(cha.passive_perception);
	ui->proficiency_spinbox->setValue(cha.proficiency);
	ui->speed_spinbox->setValue(cha.speed);
	ui->str_spinbox->setValue(cha.strength);
	ui->wis_spinbox->setValue(cha.wisdom);
}

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
