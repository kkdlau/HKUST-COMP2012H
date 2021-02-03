#include "units/Archer.h"

Archer::Archer(char id, int row, int col) : Unit{id, UnitType::Arrow, 9, 1, 5, 3, row, col} {}
Archer::~Archer() {}

void Archer::receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) {
	receive_damage_actual(raw_damage);
}

string Archer::to_string() const { return (to_string_base() + "Archer"); }

void Archer::refresh_list_item_status() {
    Unit::refresh_list_item_status();
    list_item->setText(COL_INDEX::TYPE, "Archer");
}
