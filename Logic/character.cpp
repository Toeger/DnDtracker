#include "character.h"
#include "dice.h"

#include <QJsonObject>

void Character::roll_initiative() {
	initiative = Dice::roll_d(20) + (dexterity - 10) / 2 + initiative_modifier;
}

QJsonObject Character::to_json() const {
	QJsonObject json;
	json["initiative_modifier"] = initiative_modifier;
	json["AC"] = AC;
	json["max_HP"] = max_HP;
	json["current_HP"] = current_HP;
	json["temporary_HP"] = temporary_HP;
	json["speed"] = speed;
	json["proficiency"] = proficiency;
	json["strength"] = strength;
	json["dexterity"] = dexterity;
	json["constitution"] = constitution;
	json["intelligence"] = intelligence;
	json["wisdom"] = wisdom;
	json["charisma"] = charisma;
	json["passive_perception"] = passive_perception;
	json["name"] = QString::fromStdString(name);
	return json;
}

Character Character::from_json(const QJsonObject &json) {
	Character cha;
	cha.initiative_modifier = json["initiative_modifier"].toInt();
	cha.AC = json["AC"].toInt();
	cha.max_HP = json["max_HP"].toInt();
	cha.current_HP = json["current_HP"].toInt();
	cha.temporary_HP = json["temporary_HP"].toInt();
	cha.speed = json["speed"].toInt();
	cha.proficiency = json["proficiency"].toInt();
	cha.strength = json["strength"].toInt();
	cha.dexterity = json["dexterity"].toInt();
	cha.constitution = json["constitution"].toInt();
	cha.intelligence = json["intelligence"].toInt();
	cha.wisdom = json["wisdom"].toInt();
	cha.charisma = json["charisma"].toInt();
	cha.passive_perception = json["passive_perception"].toInt();
	cha.name = json["name"].toString().toStdString();
	return cha;
}
