#include "Knight.h"

#include <iostream>
const int Knight::HEALTH = 10;
const int Knight::ATTACK = 14;
const int Knight::DEFENSE = 3;
const int Knight::ATT_RANGE = 1;
const int Knight::MOVE_RANGE = 10;
const double Knight::BE_ATT_STRENGTHER_ENEMY_SCALAR = 1.5;
const double Knight::BE_ATT_WEAKER_ENEMY_SCALAR = 0.5;
const char* Knight::NAME = "Knight";

Knight::Knight(int id, int row, int col)
	: Unit(id, Knight::TYPE, Knight::ATTACK, Knight::DEFENSE, Knight::ATT_RANGE,
		   Knight::MOVE_RANGE, row, col) {}

Knight::~Knight() {}

string Knight::to_string() const {
	return to_string_base().append(Knight::NAME);
}

void Knight::receive_damage_affinity(double raw_damage,
									 Unit::UnitType attacker_unit_type) {
	// std::cout << "Knight receive: " << raw_damage << std::endl;
	if (attacker_unit_type == STRONGER_ENEMY_UNIT_TYPE) {
		// std::cout << "Knight BE_ATT_STRENGTHER_ENEMY_SCALAR" << std::endl;

		receive_damage_actual(raw_damage * BE_ATT_STRENGTHER_ENEMY_SCALAR);
	} else if (attacker_unit_type == WEAKER_ENEMY_UNIT_TYPE) {
		// std::cout << "Knight BE_ATT_WEAKER_ENEMY_SCALAR" << std::endl;

		receive_damage_actual(raw_damage * BE_ATT_WEAKER_ENEMY_SCALAR);
	} else {
		// std::cout << "Knight NORMAL" << std::endl;

		receive_damage_actual(raw_damage);
	}
}
