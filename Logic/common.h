#ifndef COMMON_H
#define COMMON_H

namespace Common {
	struct Turns {
		int turns{};
		explicit Turns(int turns);
	};

	enum class Subturn { action, bonus_action, reaction };

	struct Duration {
		Duration(Turns turns);
		Duration(Subturn subturn);
		Turns get_turns() const;
		Subturn get_subturn() const;
		bool is_subturn() const;

		private:
		Turns turns{0};
		Subturn subturn{};
	};
}

bool operator==(const Common::Duration &lhs, const Common::Duration &rhs);
bool operator==(const Common::Duration &lhs, const int &rhs);
bool operator==(const Common::Duration &lhs, const Common::Subturn &rhs);
bool operator==(const int &lhs, const Common::Duration &rhs);
bool operator==(const Common::Subturn &lhs, const Common::Duration &rhs);

#endif // COMMON_H