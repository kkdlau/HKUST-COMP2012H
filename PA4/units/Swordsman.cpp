#include "units/Swordsman.h"

Swordsman::Swordsman(char id, int row, int col) : Unit{id, UnitType::Sword, 10, 5, 1, 5, row, col} {}
Swordsman::~Swordsman() {}

void Swordsman::receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) {
	switch (attacker_unit_type) {
	case UnitType::Spear:
		receive_damage_actual(raw_damage / 2.0);
		break;

	case UnitType::Horse:
		receive_damage_actual(raw_damage * 1.5);
		break;

	default:
		receive_damage_actual(raw_damage);
		break;
	}
}

string Swordsman::to_string() const { return (to_string_base() + "Swordsman"); }

void Swordsman::refresh_list_item_status() {
    Unit::refresh_list_item_status();
    list_item->setText(COL_INDEX::TYPE, "Swordsman");
}
