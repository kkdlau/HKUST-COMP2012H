#include "Pikeman.h"

#include <cmath>

const int Pikeman::HEALTH = 10;
const int Pikeman::ATTACK = 8;
const int Pikeman::DEFENSE = 7;
const int Pikeman::ATT_RANGE = 1;
const int Pikeman::MOVE_RANGE = 3;
const double Pikeman::BE_ATT_STRENGTHER_ENEMY_SCALAR = 1.5;
const double Pikeman::BE_ATT_WEAKER_ENEMY_SCALAR = 0.5;
const char* Pikeman::NAME = "Pikeman";

Pikeman::Pikeman(int id, int row, int col)
	: Unit(id, Pikeman::TYPE, Pikeman::ATTACK, Pikeman::DEFENSE,
		   Pikeman::ATT_RANGE, Pikeman::MOVE_RANGE, row, col) {}

Pikeman::~Pikeman() {}

string Pikeman::to_string() const {
	return to_string_base().append(Pikeman::NAME);
}

void Pikeman::receive_damage_affinity(double raw_damage,
									  Unit::UnitType attacker_unit_type) {
	if (attacker_unit_type == STRONGER_ENEMY_UNIT_TYPE) {
		receive_damage_actual(raw_damage * BE_ATT_STRENGTHER_ENEMY_SCALAR);

	} else if (attacker_unit_type == WEAKER_ENEMY_UNIT_TYPE) {
		receive_damage_actual(raw_damage * BE_ATT_WEAKER_ENEMY_SCALAR);
	} else {
		receive_damage_actual(raw_damage);
	}
}
