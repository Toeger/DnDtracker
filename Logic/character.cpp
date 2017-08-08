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
	json["level"] = level;
	json["hit_die"] = hit_die;
	json["current_HP"] = current_HP;
	json["temporary_HP"] = temporary_HP;
	json["ground_speed"] = ground_speed;
	json["swim_speed"] = swim_speed;
	json["fly_speed"] = fly_speed;
	json["other_speed"] = other_speed;
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

int Character::get_max_hp() const {
	return get_max_hp(level, hit_die, constitution);
}

int Character::get_max_hp(int level, int hit_die, int con) {
	return hit_die + (level - 1) * (hit_die + 1) / 2 + level * (con / 2 - 5);
}

Character Character::from_json(const QJsonObject &json) {
	Character cha;
	cha.initiative_modifier = json["initiative_modifier"].toInt();
	cha.AC = json["AC"].toInt();
	cha.level = json["level"].toInt();
	cha.hit_die = json["hit_die"].toInt();
	cha.current_HP = json["current_HP"].toInt();
	cha.temporary_HP = json["temporary_HP"].toInt();
	cha.ground_speed = json["ground_speed"].toInt();
	cha.swim_speed = json["swim_speed"].toInt();
	cha.fly_speed = json["fly_speed"].toInt();
	cha.other_speed = json["other_speed"].toInt();
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
