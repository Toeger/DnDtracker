#ifndef CHARACTER_H
#define CHARACTER_H

#include "class.h"
#include "skill_description.h"
#include "spellslots.h"

#include <string>
#include <vector>

struct Character {
	std::string name{};
	int initiative{};
	int initiative_modifier{};
	int AC{};
	int max_HP{};
	int current_HP{};
	int temporary_HP{};
	int speed{};
	bool reaction{};
	std::vector<Skill_description> skills{};
	std::vector<std::pair<Class, Spellslots>> spellslots{};
	int proficiency{};
	int strength{};
	int dexterity{};
	int constitution{};
	int intelligence{};
	int wisdom{};
	int charisma{};
	int passive_perception{};
	//proficiencies for skills and saving throws
	void roll_initiative();
};

#endif // CHARACTER_H