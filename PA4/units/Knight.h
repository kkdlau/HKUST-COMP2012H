#ifndef KNIGHT_H_
#define KNIGHT_H_

#include "units/Unit.h"

class Knight : public Unit {
public:
	Knight(char id, int row, int col);
	virtual ~Knight();
	virtual void receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) override;
	virtual string to_string() const override;
    virtual void refresh_list_item_status() override;  // NEW
};

#endif /* KNIGHT_H_ */
