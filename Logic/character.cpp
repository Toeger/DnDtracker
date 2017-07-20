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
