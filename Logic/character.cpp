#include "character.h"
#include "dice.h"

void Character::roll_initiative()
{
	initiative = Dice::roll_d(20) + (dexterity - 10) / 2 + initiative_modifier;
}
