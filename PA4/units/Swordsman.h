#ifndef SWORDSMAN_H_
#define SWORDSMAN_H_

#include "units/Unit.h"

class Swordsman : public Unit {
public:
	Swordsman(char id, int row, int col);
	virtual ~Swordsman();
	virtual void receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) override;
	virtual string to_string() const override;
    virtual void refresh_list_item_status() override;  // NEW
};

#endif /* SWORDSMAN_H_ */
