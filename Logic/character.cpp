#include "character.h"
#include "Utility/overloaded_function.h"
#include "dice.h"

#include <QJsonObject>
#include <boost/variant.hpp>

struct Member_string {
	boost::variant<int Character::*, QString Character::*, bool Character::*> memberCharacter{};
	const char *name{};
};

void Character::roll_initiative() {
	initiative = Dice::roll_d(20) + dexterity / 2 - 5 + initiative_modifier;
}

QJsonObject Character::to_json() const {
	QJsonObject json;
	for (const auto &member_string : get_member_strings()) {
		boost::apply_visitor(make_overloaded_function([ name = member_string.name, this, &json ](int Character::*number) { json[name] = this->*number; },
													  [ name = member_string.name, this, &json ](QString Character::*string) { json[name] = this->*string; },
													  [ name = member_string.name, this, &json ](bool Character::*boolean) { json[name] = this->*boolean; }),
							 member_string.memberCharacter);
	}
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
	for (const auto &member_string : get_member_strings()) {
		boost::apply_visitor(
			make_overloaded_function([ name = member_string.name, &cha, &json ](int Character::*number) { cha.*number = json[name].toInt(); },
									 [ name = member_string.name, &cha, &json ](QString Character::*string) { cha.*string = json[name].toString(); },
									 [ name = member_string.name, &cha, &json ](bool Character::*boolean) { cha.*boolean = json[name].toBool(); }),
			member_string.memberCharacter);
	}
	return cha;
}

std::vector<Member_string> Character::get_member_strings() {
	return {
		{&Character::initiative_modifier, "initiative_modifier"},
		{&Character::AC, "AC"},
		{&Character::level, "level"},
		{&Character::hit_die, "hit_die"},
		{&Character::current_HP, "current_HP"},
		{&Character::temporary_HP, "temporary_HP"},
		{&Character::ground_speed, "ground_speed"},
		{&Character::swim_speed, "swim_speed"},
		{&Character::fly_speed, "fly_speed"},
		{&Character::other_speed, "other_speed"},
		{&Character::proficiency, "proficiency"},
		{&Character::strength, "strength"},
		{&Character::dexterity, "dexterity"},
		{&Character::constitution, "constitution"},
		{&Character::intelligence, "intelligence"},
		{&Character::wisdom, "wisdom"},
		{&Character::charisma, "charisma"},
		{&Character::passive_perception, "passive_perception"},
		{&Character::name, "name"},
		{&Character::size, "size"},
		{&Character::type, "type"},
		{&Character::tag, "tag"},
		{&Character::alignment, "alignment"},
		{&Character::challenge_rating, "challenge_rating"},
		{&Character::armor, "armor"},
		{&Character::experience, "experience"},
	};
}
