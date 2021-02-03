#include "Player.h"

Player::Player(const string& name, int num_units, Unit* const* const units) : name{name}, num_units{num_units}, units{units} {}
Player::~Player() {
	for (int i = 0; i < num_units; i += 1) {
		delete units[i];
	}
	delete[] units;
    delete list_item;
}

Unit* Player::get_unit_by_id(char id) const {
	for (int i = 0; i < num_units; i += 1) {
		if (units[i]->get_id() == id) { return units[i]; }
	}
	return nullptr;
}

bool Player::has_units_alive() const {
	for (int i = 0; i < num_units; i += 1) {
		if (units[i]->is_alive()) { return true; }
	}
	return false;
}

bool Player::has_units_ready() const {
	for (int i = 0; i < num_units; i += 1) {
		if (units[i]->is_alive() && units[i]->is_ready()) { return true; }
	}
	return false;
}

void Player::ready_all_units() const {
    for (int i = 0; i < num_units; i += 1) {
        if (units[i]->is_alive()) { units[i]->begin_turn(); }
	}
}

const string& Player::get_name() const { return name; }
int Player::get_num_units() const { return num_units; }
Unit* const* const Player::get_units() const { return units; }

bool Player::operator==(const Player& rhs) const { return (this == &rhs); }

void Player::register_list_item(QTreeWidgetItem *_item) {
    list_item = _item;
}

void Player::refresh_list_item_status() {
    for (int i = 0; i < num_units; i += 1) {
        units[i]->refresh_list_item_status();
    }
}
