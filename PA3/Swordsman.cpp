#include "Swordsman.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cmath>

const int Swordsman::HEALTH = 10;
const int Swordsman::ATTACK = 10;
const int Swordsman::DEFENSE = 5;
const int Swordsman::ATT_RANGE = 1;
const int Swordsman::MOVE_RANGE = 5;
const double Swordsman::BE_ATT_STRENGTHER_ENEMY_SCALAR = 1.5;
const double Swordsman::BE_ATT_WEAKER_ENEMY_SCALAR = 0.5;
const char* Swordsman::NAME = "Swordsman";

Swordsman::Swordsman(int id, int row, int col)
	: Unit(id, Swordsman::TYPE, Swordsman::ATTACK, Swordsman::DEFENSE,
		   Swordsman::ATT_RANGE, Swordsman::MOVE_RANGE, row, col) {}

Swordsman::~Swordsman() {}

string Swordsman::to_string() const {
	return to_string_base().append(Swordsman::NAME);
}

void Swordsman::receive_damage_affinity(double raw_damage,
										Unit::UnitType attacker_unit_type) {
	if (attacker_unit_type == STRONGER_ENEMY_UNIT_TYPE) {
		receive_damage_actual(raw_damage *
							  Swordsman::BE_ATT_STRENGTHER_ENEMY_SCALAR);

	} else if (attacker_unit_type == WEAKER_ENEMY_UNIT_TYPE) {
		receive_damage_actual(raw_damage *
							  Swordsman::BE_ATT_WEAKER_ENEMY_SCALAR);
	} else {
		receive_damage_actual(raw_damage);
	}
}
