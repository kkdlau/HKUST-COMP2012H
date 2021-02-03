#ifndef UNIT_H_
#define UNIT_H_

#include <string>
using std::string;

#include <QGraphicsPixmapItem>
#include <QTreeWidgetItem>

enum COL_INDEX {
    STATUS = 0,
    TYPE,
    HEALTH,
    ATT,
    DEF,
    ATT_RANGE,
    MOVE_RANGE
};

class Unit {
public:
	enum class UnitType { Sword, Spear, Horse, Arrow };
	static const int MAX_HEALTH{10};
	static const int HEAL_RATE{3};

	virtual ~Unit();

	virtual void begin_turn();
	virtual void end_turn();
	virtual void move_delta(int delta_row, int delta_col);
	virtual void attack_unit(Unit* const defender);
	virtual void receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) = 0;
	virtual void heal();

	char get_id() const;
	UnitType get_unit_type() const;
	int get_attack_range() const;
	int get_movement_range() const;
	int get_position_row() const;
	int get_position_col() const;
    QGraphicsPixmapItem *get_view() const;
	bool is_ready() const;
	bool is_alive() const;

	bool operator==(const Unit& rhs) const;
    virtual string to_string() const = 0;

    void register_view(QGraphicsPixmapItem *_view);  // NEW
    void register_list_item(QTreeWidgetItem *_item);  // NEW

    void refresh_view_position();  // NEW
    virtual void refresh_list_item_status();  // NEW

protected:
	Unit(char id, UnitType unit_type, int attack, int defense, int attack_range, int movement_range, int row, int col);

	void receive_damage_actual(double actual_damage);
	string to_string_base() const;

	const char id;
	const UnitType unit_type;
	int health{MAX_HEALTH};
	const int attack;
	const int defense;
	const int attack_range;
	const int movement_range;
	int position_row, position_col;
	bool ready_state{false};

    QGraphicsPixmapItem *view{nullptr};  // NEW
    QTreeWidgetItem *list_item{nullptr};  // NEW
};

#endif /* UNIT_H_ */
