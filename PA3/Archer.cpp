#include "Archer.h"

const int Archer::HEALTH = 10;
const int Archer::ATTACK = 9;
const int Archer::DEFENSE = 1;
const int Archer::ATT_RANGE = 5;
const int Archer::MOVE_RANGE = 3;
const double Archer::BE_ATT_STRENGTHER_ENEMY_SCALAR = 1.5;
const double Archer::BE_ATT_WEAKER_ENEMY_SCALAR = 0.5;
const char* Archer::NAME = "Archer";

Archer::Archer(int id, int row, int col)
	: Unit(id, Archer::TYPE, Archer::ATTACK, Archer::DEFENSE, Archer::ATT_RANGE,
		   Archer::MOVE_RANGE, row, col) {}

Archer::~Archer() {}

string Archer::to_string() const {
	return to_string_base().append(Archer::NAME);
}

void Archer::receive_damage_affinity(double raw_damage,
									 Unit::UnitType attacker_unit_type) {
	receive_damage_actual(raw_damage);
}
