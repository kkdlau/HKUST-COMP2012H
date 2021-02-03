#ifndef _PIKEMAN_H_
#define _PIKEMAN_H_

#include "Unit.h"

#define PIKEMAN_DATA_ATBLE(X)                                                \
	X(HEALTH, const int, 10)                                                 \
	X(ATTACK, const int, 8)                                                  \
	X(DEFENSE, const int, 7)                                                 \
	X(ATT_RANGE, const int, 1)                                               \
	X(MOVE_RANGE, const int, 3)                                              \
	X(BE_ATT_STRENGTHER_ENEMY_SCALAR, const double, 1.5)                     \
	X(BE_ATT_WEAKER_ENEMY_SCALAR, const double, 0.5)                         \
	X(NAME, const char*, "Pikeman")                                          \
	X(STRONGER_ENEMY_UNIT_TYPE, const Unit::UnitType, Unit::UnitType::Sword) \
	X(WEAKER_ENEMY_UNIT_TYPE, const Unit::UnitType, Unit::UnitType::Horse)

class Pikeman : public Unit {
   private:
   public:
	static const int HEALTH;
	static const int ATTACK;
	static const int DEFENSE;
	static const int ATT_RANGE;
	static const int MOVE_RANGE;
	static const double BE_ATT_STRENGTHER_ENEMY_SCALAR;
	static const double BE_ATT_WEAKER_ENEMY_SCALAR;
	static const char* NAME;
	static const UnitType TYPE = UnitType::Spear;
	static const UnitType STRONGER_ENEMY_UNIT_TYPE = UnitType::Sword;
	static const UnitType WEAKER_ENEMY_UNIT_TYPE = UnitType::Horse;

	Pikeman(int id, int row, int col);
	~Pikeman();

	virtual string to_string() const;
	virtual void receive_damage_affinity(double raw_damage,
										 UnitType attacker_unit_type);
};

#endif