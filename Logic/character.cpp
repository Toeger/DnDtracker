#include "character.h"
#include "dice.h"

#include <QJsonObject>

void Character::roll_initiative() {
	initiative = Dice::roll_d(20) + dexterity / 2 - 5 + initiative_modifier;
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
	json["name"] = name;
	json["size"] = size;
	json["type"] = type;
	json["tag"] = tag;
	json["alignment"] = alignment;
	json["challenge_rating"] = challenge_rating;
	json["armor"] = armor;
	json["experience"] = experience;
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
	cha.name = json["name"].toString();
	cha.size = json["size"].toString();
	cha.type = json["type"].toString();
	cha.tag = json["tag"].toString();
	cha.alignment = json["alignment"].toString();
	cha.challenge_rating = json["challenge_rating"].toString();
	cha.armor = json["armor"].toString();
	cha.experience = json["experience"].toInt();
	return cha;
}
