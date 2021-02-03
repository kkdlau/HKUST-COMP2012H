#ifndef ARCHER_H_
#define ARCHER_H_

#include "units/Unit.h"

class Archer : public Unit {
public:
	Archer(char id, int row, int col);
	virtual ~Archer();
	virtual void receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) override;
	virtual string to_string() const override;
    virtual void refresh_list_item_status() override;  // NEW
};

#endif /* ARCHER_H_ */
