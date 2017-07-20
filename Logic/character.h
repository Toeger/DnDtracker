#ifndef CHARACTER_H
#define CHARACTER_H

#include "class.h"
#include "skill_description.h"
#include "spellslots.h"

#include <string>
#include <vector>

class QJsonObject;

struct Character {
	std::string name{};
	int initiative{};
	int initiative_modifier{};
	int AC{};
	int max_HP{};
	int current_HP{};
	int temporary_HP{};
	int speed{};
	int proficiency{};
	int strength{};
	int dexterity{};
	int constitution{};
	int intelligence{};
	int wisdom{};
	int charisma{};
	int passive_perception{};
	bool reaction{};
	bool action{};
	std::vector<Skill_description> skills{};
	std::vector<std::pair<Class, Spellslots>> spellslots{};
	//proficiencies for skills and saving throws
	void roll_initiative();
	QJsonObject to_json() const;
	static Character from_json(const QJsonObject &json);
};

#endif // CHARACTER_H