#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
using std::string;

#include "Unit.h"

class Player {
   public:
	Player(const string& name, int num_units, Unit* const* const units);
	~Player();

	Unit* get_unit_by_id(char id) const;
	bool has_units_alive() const;
	bool has_units_ready() const;
	void ready_all_units() const;

	const string& get_name() const;
	int get_num_units() const;
	Unit* const* const get_units() const;

   private:
	const string name;
	const int num_units;
	Unit* const* const units;
};

#endif /* PLAYER_H_ */
