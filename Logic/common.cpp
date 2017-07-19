#include "common.h"

Common::Turns::Turns(int turns)
	: turns{turns} {}

bool operator==(const Common::Duration &lhs, const Common::Duration &rhs) {
	if (lhs.is_subturn() == false) {
		return lhs.get_turns() == rhs.get_turns();
	}
	return lhs.get_subturn() == rhs.get_subturn();
}

Common::Duration::Duration(Common::Turns turns)
	: turns{turns} {}

Common::Duration::Duration(Common::Subturn subturn)
	: subturn{subturn} {}

Common::Turns Common::Duration::get_turns() const {
	return turns;
}

Common::Subturn Common::Duration::get_subturn() const {
	return subturn;
}

bool Common::Duration::is_subturn() const {
	return turns == 0;
}

bool operator==(const Common::Duration &lhs, const int &rhs) {
	return lhs.is_subturn() == false && lhs.get_turns() == rhs;
}

bool operator==(const Common::Duration &lhs, const Common::Subturn &rhs) {
	return lhs.is_subturn() && lhs.get_subturn() == rhs;
}

bool operator==(const int &lhs, const Common::Duration &rhs) {
	return rhs == lhs;
}

bool operator==(const Common::Subturn &lhs, const Common::Duration &rhs) {
	return rhs == lhs;
}
