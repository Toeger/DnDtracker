#include "dice.h"

#include <initializer_list>
#include <random>

static std::mt19937_64 rng{std::random_device{}()};

int Dice::roll_d(int die_size) {
	return std::uniform_int_distribution<>{1, die_size}(rng);
}
