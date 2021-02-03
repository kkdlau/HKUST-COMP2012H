#include "units/Pikeman.h"

Pikeman::Pikeman(char id, int row, int col) : Unit{id, UnitType::Spear, 8, 7, 1, 3, row, col} {}
Pikeman::~Pikeman() {}

void Pikeman::receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) {
	switch (attacker_unit_type) {
	case UnitType::Sword:
		receive_damage_actual(raw_damage * 1.5);
		break;

	case UnitType::Horse:
		receive_damage_actual(raw_damage / 2.0);
		break;

	default:
		receive_damage_actual(raw_damage);
		break;
	}
}

string Pikeman::to_string() const { return (to_string_base() + "Pikeman"); }

void Pikeman::refresh_list_item_status() {
    Unit::refresh_list_item_status();
    list_item->setText(COL_INDEX::TYPE, "Pikeman");
}
