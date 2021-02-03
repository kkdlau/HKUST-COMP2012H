#ifndef UNIT_H_
#define UNIT_H_

#include <string>
using std::string;

#define STATIC_VAR_DECLAR(alias, type, ...) static type alias;

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
	virtual void receive_damage_affinity(double raw_damage,
										 UnitType attacker_unit_type) = 0;
	virtual void heal();

	char get_id() const;
	UnitType get_unit_type() const;
	int get_attack_range() const;
	int get_movement_range() const;
	int get_position_row() const;
	int get_position_col() const;
	bool is_ready() const;
	bool is_alive() const;
	virtual string to_string() const = 0;

   protected:
	Unit(char id, UnitType unit_type, int attack, int defense, int attack_range,
		 int movement_range, int row, int col);

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
};

#endif /* UNIT_H_ */
