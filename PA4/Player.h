#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
using std::string;

#include <QTreeWidgetItem>

#include "units/Unit.h"

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

	bool operator==(const Player& rhs) const;

    void register_list_item(QTreeWidgetItem *_item);  // NEW
    void refresh_list_item_status();  // NEW

private:
	const string name;
    const int num_units{0};
    Unit* const* const units{nullptr};

    QTreeWidgetItem *list_item{nullptr};  // NEW
};

#endif /* PLAYER_H_ */
