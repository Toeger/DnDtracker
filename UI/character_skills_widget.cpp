#include "character_skills_widget.h"
#include "ui_character_skills_widget.h"

Character_skills_widget::Character_skills_widget(QWidget *parent, Character_selector_widget *target)
	: QWidget{parent}
	, ui{new Ui::Character_skills_widget}
	, target{target} {
	ui->setupUi(this);

	for (const auto &saving_throw : {
			 "Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma",
		 }) {
		//6
		(void)saving_throw;
	}
	for (const auto &skill : {"Acrobatics", "Animal Handling", "Arcana", "Athletics", "Deception", "History", "Insight", "Intimidation", "Investigation",
							  "Medicine", "Nature", "Perception", "Performance", "Persuasion", "Religion", "Sleight of Hand", "Stealth", "Survival"}) {
		//None, Proficient, Expert | Advantage, Disadvantage //18
		(void)skill;
	}
	for (const auto &condition : {
			 "Blinded", "Charmed", "Deafened", "Exhausted", "Grappled", "Incapacitated", "Paralyzed", "Petrified", "Poisoned", "Knocked Prone", "Frightened",
			 "Stunned", "Knocked Unconscious",
		 }) {
		//advantage on saving throws against being | immune to being //13
		(void)condition;
	}
	for (const auto &sense : {"Smell", "Hearing", "Sight"}) {
		//advantage on perception when using smell, hearing, sight //3
		(void)sense;
	}
	show();
}

Character_skills_widget::~Character_skills_widget() {
	delete ui;
}
