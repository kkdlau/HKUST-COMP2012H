#include <iomanip>
using std::setw;
#include <sstream>
using std::stringstream;
#include <cmath>
using std::abs;
using std::round;
#include <QDebug>

#include "units/Unit.h"

// public
Unit::~Unit() {
    delete view;
    delete list_item;
}

void Unit::begin_turn() { ready_state = true; refresh_list_item_status(); }
void Unit::end_turn() { ready_state = false; refresh_list_item_status(); }

void Unit::move_delta(int delta_row, int delta_col) {
	position_row += delta_row;
	position_col += delta_col;
    refresh_view_position();
}

void Unit::attack_unit(Unit* const defender) {
	double defender_raw_damage_received = double(this->attack - defender->defense) * (this->health / double(MAX_HEALTH));
	double attacker_raw_damage_received = double(defender->attack - this->defense) * (defender->health / double(MAX_HEALTH));

	defender->receive_damage_affinity(defender_raw_damage_received, this->unit_type);
	int distance = abs(this->position_row - defender->position_row) + abs(this->position_col - defender->position_col);
	if (distance <= defender->attack_range) { this->receive_damage_affinity(attacker_raw_damage_received, defender->unit_type); }
}

void Unit::heal() {
	health += HEAL_RATE;
	if (health > MAX_HEALTH) { health = MAX_HEALTH; }
    refresh_list_item_status();
}

char Unit::get_id() const { return id; }
Unit::UnitType Unit::get_unit_type() const { return unit_type; }
int Unit::get_attack_range() const { return attack_range; }
int Unit::get_movement_range() const { return movement_range; }
int Unit::get_position_row() const { return position_row; }
int Unit::get_position_col() const { return position_col; }
bool Unit::is_ready() const { return ready_state; }
bool Unit::is_alive() const { return (health > 0); }

bool Unit::operator==(const Unit& rhs) const { return (this == &rhs); }

void Unit::register_view(QGraphicsPixmapItem *_view) {
    view = _view;
    refresh_view_position();
}

QGraphicsPixmapItem * Unit::get_view() const {
    return view;
}
void Unit::register_list_item(QTreeWidgetItem *_item) {
    list_item = _item;
    refresh_list_item_status();
}

void Unit::refresh_view_position() {
    if (health <= 0) view->setVisible(false);
    else {
        view->setVisible(true);
        view->setOffset(QPointF{position_col * 16.0f, position_row * 16.0f});
    }
}

void Unit::refresh_list_item_status() {
    list_item->setText(COL_INDEX::STATUS, is_alive()? (is_ready()? "READY": "DONE"): "DEAD");
    list_item->setText(COL_INDEX::HEALTH, QString::number(health));
    list_item->setText(COL_INDEX::ATT, QString::number(attack));
    list_item->setText(COL_INDEX::DEF, QString::number(defense));
    list_item->setText(COL_INDEX::ATT_RANGE, QString::number(attack_range));
    list_item->setText(COL_INDEX::MOVE_RANGE, QString::number(movement_range));

}

// protected
Unit::Unit(char unit_id, UnitType unit_type, int attack, int defense, int attack_range, int movement_range, int row, int col)
	: id{unit_id}, unit_type{unit_type}, attack{attack}, defense{defense}, attack_range{attack_range}, movement_range{movement_range}, position_row{row}, position_col{col} {}

void Unit::receive_damage_actual(double actual_damage) {
	health -= round(actual_damage);
	if (health < 0) { health = 0; }
    refresh_list_item_status();
    refresh_view_position();
}

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
		if (is_ready()) { ss << "READY  "; }
		else { ss << "DONE   "; }
	} else { ss << "DEAD   "; }
	return ss.str();
}
