#include "units/Knight.h"

Knight::Knight(char id, int row, int col) : Unit{id, UnitType::Horse, 14, 3, 1, 10, row, col} {}
Knight::~Knight() {}

void Knight::receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) {
	switch (attacker_unit_type) {
	case UnitType::Sword:
		receive_damage_actual(raw_damage / 2.0);
		break;

	case UnitType::Spear:
		receive_damage_actual(raw_damage * 1.5);
		break;

	default:
		receive_damage_actual(raw_damage);
		break;
	}
}

string Knight::to_string() const { return (to_string_base() + "Knight"); }

void Knight::refresh_list_item_status() {
    Unit::refresh_list_item_status();
    list_item->setText(COL_INDEX::TYPE, "Knight");
}
