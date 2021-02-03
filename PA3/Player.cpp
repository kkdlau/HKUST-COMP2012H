#include "Player.h"

#include <cstring>

Player::Player(const string& name, int num_units, Unit* const* const units)
	: name{name}, num_units{num_units}, units{units} {
	/**
	 * Placeholder
	 */
}

Player::~Player() {
	Unit* const* u = get_units();
	const int unit_length = get_num_units();
	for (int i = 0; i < unit_length; ++i) {
		delete u[i];
	}
}

Unit* Player::get_unit_by_id(char id) const {
	Unit* const* u = get_units();
	const int unit_length = get_num_units();
	for (int i = 0; i < unit_length; ++i) {
		if (units[i]->get_id() == id) return units[i];
	}
	return nullptr;
}

bool Player::has_units_alive() const {
	Unit* const* u = get_units();
	const int unit_length = get_num_units();
	for (int i = 0; i < unit_length; ++i) {
		if (u[i]->is_alive()) return true;
	}
	return false;
}

bool Player::has_units_ready() const {
	Unit* const* u = get_units();
	const int unit_length = get_num_units();
	for (int i = 0; i < unit_length; ++i) {
		if (u[i]->is_ready()) return true;
	}
	return false;
}

void Player::ready_all_units() const {
	Unit* const* u = get_units();
	const int unit_length = get_num_units();
	for (int i = 0; i < unit_length; ++i) {
		u[i]->begin_turn();
	}
}

const string& Player::get_name() const { return name; }

int Player::get_num_units() const { return num_units; }

Unit* const* const Player::get_units() const { return units; }
