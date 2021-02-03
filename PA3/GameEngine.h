#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <string>
using std::string;

#include "Archer.h"
#include "GameMap.h"
#include "Knight.h"
#include "Pikeman.h"
#include "Player.h"
#include "Swordsman.h"
#include "Unit.h"

class GameEngine {
   public:
	GameEngine();
	~GameEngine();
	void run_game();

   private:
	void load_players_and_units(const string& filename);
	void print_players_and_units() const;
	void print_game_over() const;

	void process_player_turns();
	void select_unit();
	void process_unit_movement_phase();
	void process_unit_attack_phase();
	bool is_game_over() const;

	GameMap game_map;
	int num_players;
	Player** players;
	Player* current_player;
	Unit* current_unit;
};

#endif /* GAMEENGINE_H_ */
