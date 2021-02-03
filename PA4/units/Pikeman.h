#ifndef PIKEMAN_H_
#define PIKEMAN_H_

#include "units/Unit.h"

class Pikeman : public Unit {
public:
	Pikeman(char id, int row, int col);
	virtual ~Pikeman();
	virtual void receive_damage_affinity(double raw_damage, UnitType attacker_unit_type) override;
	virtual string to_string() const override;
    virtual void refresh_list_item_status() override;  // NEW
};

#endif /* PIKEMAN_H_ */
