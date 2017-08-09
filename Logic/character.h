#ifndef CHARACTER_H
#define CHARACTER_H

#include "class.h"
#include "skill_description.h"
#include "spellslots.h"

#include <QString>
#include <vector>

class QJsonObject;
struct Member_string;

struct Character {
	QString name{};
	int initiative{};
	int initiative_modifier{};
	int AC{};
	int current_HP{};
	int temporary_HP{};
	int ground_speed{};
	int swim_speed{};
	int fly_speed{};
	int other_speed{};
	int proficiency{};
	int strength{};
	int dexterity{};
	int constitution{};
	int intelligence{};
	int wisdom{};
	int charisma{};
	int passive_perception{};
	int experience{};
	int level{};
	int hit_die{};
	bool reaction{};
	QString size{};
	QString type{};
	QString tag{};
	QString alignment{};
	QString challenge_rating{};
	QString armor{};
	QString concentration{};
	std::vector<Skill_description> skills{};
	std::vector<std::pair<Class, Spellslots>> spellslots{};
	//proficiencies for skills and saving throws
	void roll_initiative();
	QJsonObject to_json() const;
	int get_max_hp() const;
	static int get_max_hp(int level, int hit_die, int con);
	static Character from_json(const QJsonObject &json);

	private:
	static std::vector<Member_string> get_member_strings();
};

#endif // CHARACTER_H