#include <iomanip>
using std::setw;
#include <sstream>
using std::stringstream;
#include <cmath>
using std::abs;
using std::round;
#include <iostream>
using std::cout;
using std::endl;
#include "Unit.h"

inline unsigned int manhattan_dist(int x, int y) { return abs(x) + abs(y); }
inline int delta(int x1, int x2) { return x1 - x2; }

string Unit::to_string_base() const {
	stringstream ss;
	ss << "[" << id << "]  ";
	ss << "H:" << setw(2) << health << "  ";
	ss << "A:" << setw(2) << attack << "  ";
	ss << "D:" << setw(2) << defense << "  ";
	ss << "AR:" << setw(2) << attack_range << "  ";
	ss << "MR:" << setw(2) << movement_range << "  ";
	ss << "row:" << setw(2) << position_row << "  ";
	ss << "col:" << setw(2) << position_col << "  ";
	if (is_alive()) {
		if (is_ready()) {
			ss << "READY  ";
		} else {
			ss << "DONE   ";
		}
	} else {
		ss << "DEAD   ";
	}
	return ss.str();
}
Unit::Unit::~Unit() {}

char Unit::get_id() const { return id; }

Unit::UnitType Unit::get_unit_type() const { return unit_type; }

int Unit::get_attack_range() const { return attack_range; }

int Unit::get_movement_range() const { return movement_range; }

int Unit::get_position_row() const { return position_row; }

int Unit::get_position_col() const { return position_col; }

bool Unit::is_ready() const { return ready_state; }

bool Unit::is_alive() const { return health > 0; }

Unit::Unit(char id, UnitType unit_type, int attack, int defense,
		   int attack_range, int movement_range, int row, int col)
	: id{id},
	  unit_type{unit_type},
	  attack{attack},
	  defense{defense},
	  attack_range{attack_range},
	  movement_range{movement_range},
	  position_row{row},
	  position_col{col} {}

void Unit::begin_turn() { ready_state = is_alive(); }
void Unit::end_turn() { ready_state = false; }

void Unit::move_delta(int delta_row, int delta_col) {
	position_row = get_position_row() + delta_row;
	position_col = get_position_col() + delta_col;
}

void Unit::attack_unit(Unit* const defender) {
	const double raw_attack =
		(attack - defender->defense) * (double(health) / MAX_HEALTH);
	const double defender_raw_attack =
		(defender->attack - defense) *
		(double(defender->health) / defender->MAX_HEALTH);

	// cout << "raw attack:" << raw_attack << endl;
	// cout << "counter raw attack:" << defender_raw_attack << endl;

	defender->receive_damage_affinity(raw_attack, get_unit_type());

	int drow = delta(position_row, defender->position_row);
	int dcol = delta(position_col, defender->position_col);
	if (manhattan_dist(drow, dcol) <= defender->get_attack_range())
		this->receive_damage_affinity(defender_raw_attack,
									  defender->get_unit_type());
}

void Unit::heal() {
	health += HEAL_RATE;
	if (health > MAX_HEALTH) health = MAX_HEALTH;
}

void Unit::receive_damage_actual(double actual_damage) {
	health -= int(round(actual_damage));
	if (health < 0) health = 0;
}
