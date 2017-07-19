#ifndef SKILL_DESCRIPTION_H
#define SKILL_DESCRIPTION_H

#include "common.h"

#include <string>

struct Skill_description {
	private:
	std::string name{};
	Common::Duration duration{Common::Subturn::action};

};

#endif // SKILL_DESCRIPTION_H