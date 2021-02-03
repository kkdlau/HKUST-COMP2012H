#include <fstream>
using std::ifstream;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <limits>
#include <string>
using std::string;
#include "GameEngine.h"

// public
GameEngine::GameEngine() {}
GameEngine::~GameEngine() { delete[] players; }

void GameEngine::run_game() {
	// Initialization.
	string game_map_filename;
	cout << "Enter Game Map textfile: ";
	cin >> game_map_filename;
	game_map.load_terrain_map(game_map_filename);
	game_map.render_and_print_display_map(players, 0);
	cout << endl;

	string players_units_filename;
	cout << "Enter Players and Units textfile: ";
	cin >> players_units_filename;
	load_players_and_units(players_units_filename);
	cout << endl;
	game_map.render_and_print_display_map(players, num_players);
	cout << endl;
	print_players_and_units();

	cout << "Start the game? Y/N: ";
	char yes_or_no;
	cin >> yes_or_no;
	if ((yes_or_no == 'Y') || (yes_or_no == 'y')) {
		// Gameplay Loop.
		cout << "GAME START" << endl;
		process_player_turns();	 // Game Over check inside.
		print_game_over();
	}
}

// private
void GameEngine::load_players_and_units(const string& filename) {
	ifstream players_and_units_file(filename);

	players_and_units_file >> num_players;
	players = new Player*[num_players];

	for (int p = 0; p < num_players; p += 1) {
		string name;
		int num_units;
		players_and_units_file >> name >> num_units;
		// cout << name << " " << num_units << endl;
		Unit** units = new Unit*[num_units];

		for (int u = 0; u < num_units; u += 1) {
			char id;
			string unit_type_string;
			int row, col;
			players_and_units_file >> id >> unit_type_string >> row >> col;
			// cout << id << " " << unit_type_string << " " << row << " " << col
			// << endl;

			Unit* unit = nullptr;
			if (unit_type_string == "Swordsman") {
				unit = new Swordsman{id, row, col};
			} else if (unit_type_string == "Pikeman") {
				unit = new Pikeman{id, row, col};
			} else if (unit_type_string == "Knight") {
				unit = new Knight{id, row, col};
			} else if (unit_type_string == "Archer") {
				unit = new Archer{id, row, col};
			}
			units[u] = unit;
			// cout << unit->to_string() << endl;
			game_map.update_terrain_map(row, col,
										GameMap::TerrainState::OCCUPIED);
		}
		players[p] = new Player{name, num_units, units};
	}
}

void GameEngine::print_players_and_units() const {
	for (int p = 0; p < num_players; p += 1) {
		cout << players[p]->get_name() << ":" << endl;
		for (int u = 0; u < players[p]->get_num_units(); u += 1) {
			cout << '\t' << players[p]->get_units()[u]->to_string() << endl;
		}
		cout << endl;
	}
}

void GameEngine::print_game_over() const {
	cout << "\n\n" << endl;
	game_map.render_and_print_display_map(players, num_players);
	cout << endl;
	print_players_and_units();
	cout << endl;
	cout << "GAME OVER" << endl;

	for (int i = 0; i < num_players; i += 1) {
		if (players[i]->has_units_alive()) {
			cout << players[i]->get_name() << " WON" << endl;
		} else {
			cout << players[i]->get_name() << " LOST" << endl;
		}
	}
	cout << endl;
}

void GameEngine::process_player_turns() {
	while (true) {
		for (int p = 0; p < num_players; p += 1) {
			if (players[p]->has_units_alive()) {
				current_player = players[p];
				cout << current_player->get_name() << "'s Turn:" << endl;

				current_player->ready_all_units();
				while (current_player->has_units_ready()) {
					cout << endl;
					game_map.render_and_print_display_map(players, num_players);
					cout << endl;
					print_players_and_units();

					select_unit();
					cout << "1. Move & Attack\n";
					cout << "2. Heal\n";
					cout << "3. Cancel\n";
					char choice;
					cin >> choice;

					switch (choice) {
						case '1':
							process_unit_movement_phase();
							process_unit_attack_phase();
							current_unit->end_turn();
							if (is_game_over()) {
								return;
							}  // Units could only die after attacking.
							   // Event-based Programming would be more
							   // efficient.
							break;

						case '2':
							current_unit->heal();
							current_unit->end_turn();
							break;

						default:
							break;
					}
				}
				cout << "\n\n" << endl;
			}
		}
	}
}

void GameEngine::select_unit() {
	string selection_units_id{""};
	for (int i = 0; i < current_player->get_num_units(); i += 1) {
		Unit* unit = current_player->get_units()[i];
		if (unit->is_alive() && unit->is_ready()) {
			selection_units_id += unit->get_id();
		}
	}

	char selection_id = 0;
	do {
		cout << "Select Unit [" << selection_units_id << "]: ";
		cin >> selection_id;
		if (selection_id == 0) {
			cout << "do";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while (selection_units_id.find(selection_id) ==
			 string::npos);	 // string::npos signifies not found.
	current_unit = current_player->get_unit_by_id(selection_id);
}

void GameEngine::process_unit_movement_phase() {
	int starting_row = current_unit->get_position_row();
	int starting_col = current_unit->get_position_col();
	int delta_row, delta_col;
	do {
		cout
			<< "Enter movement delta (row col), input (0 0) to Hold Position: ";
		cin >> delta_row >> delta_col;
		if ((delta_row == 0) && (delta_col == 0)) {
			return;
		}
	} while (!game_map.is_valid_path(starting_row, starting_col, delta_row,
									 delta_col,
									 current_unit->get_movement_range()));

	current_unit->move_delta(delta_row, delta_col);
	game_map.update_terrain_map(starting_row, starting_col,
								GameMap::TerrainState::EMPTY);
	game_map.update_terrain_map(starting_row + delta_row,
								starting_col + delta_col,
								GameMap::TerrainState::OCCUPIED);

	cout << endl;
	game_map.render_and_print_display_map(players, num_players);
	cout << endl;
	print_players_and_units();
}

void GameEngine::process_unit_attack_phase() {
	string enemy_units_in_range_id{""};
	for (int p = 0; p < num_players; p += 1) {
		if (players[p] == current_player) {
			continue;
		}
		for (int u = 0; u < players[p]->get_num_units(); u += 1) {
			Unit* enemy_unit = players[p]->get_units()[u];
			if (enemy_unit->is_alive()) {
				int distance_row = ((current_unit->get_position_row() >
									 enemy_unit->get_position_row())
										? (current_unit->get_position_row() -
										   enemy_unit->get_position_row())
										: (enemy_unit->get_position_row() -
										   current_unit->get_position_row()));
				int distance_col = ((current_unit->get_position_col() >
									 enemy_unit->get_position_col())
										? (current_unit->get_position_col() -
										   enemy_unit->get_position_col())
										: (enemy_unit->get_position_col() -
										   current_unit->get_position_col()));
				if ((distance_row + distance_col) <=
					current_unit->get_attack_range()) {
					enemy_units_in_range_id += enemy_unit->get_id();
				}
			}
		}
	}

	if (enemy_units_in_range_id == "") {
		cout << "No Enemy Units within Attack Range. Ending Turn for Unit "
			 << current_unit->get_id() << "." << endl;
		return;
	}

	cout << "Select Enemy Unit to Attack [" << enemy_units_in_range_id
		 << "], or input / to Skip Attack: ";
	char selection_id;
	cin >> selection_id;
	if (enemy_units_in_range_id.find(selection_id) != string::npos) {
		Unit* enemy_unit;
		for (int p = 0; p < num_players; p += 1) {
			if (players[p] == current_player) {
				continue;
			}
			enemy_unit = players[p]->get_unit_by_id(selection_id);
			if (enemy_unit != nullptr) {
				break;
			}
		}

		current_unit->attack_unit(enemy_unit);
		if (!current_unit->is_alive()) {
			game_map.update_terrain_map(current_unit->get_position_row(),
										current_unit->get_position_col(),
										GameMap::TerrainState::EMPTY);
		}
		if (!enemy_unit->is_alive()) {
			game_map.update_terrain_map(enemy_unit->get_position_row(),
										enemy_unit->get_position_col(),
										GameMap::TerrainState::EMPTY);
		}
	}
}

bool GameEngine::is_game_over() const {
	int alive_team = 0;
	for (int i = 0; i < num_players; ++i) {
		Player* const p = players[i];
		if (p->has_units_alive()) ++alive_team;
		if (alive_team >= 2) return false;
	}
	return true;
}
