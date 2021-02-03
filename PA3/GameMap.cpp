#include <fstream>
using std::ifstream;
using std::noskipws;
using std::ws;
#include <iostream>
using std::cout;
#include "GameMap.h"

inline unsigned int manhattan_dist(int x, int y) { return abs(x) + abs(y); }

GameMap::GameMap() {}
GameMap::~GameMap() {
	delete[] terrain_map;
	delete[] display_map;
}

void GameMap::load_terrain_map(const string& filename) {
	ifstream terrain_map_file(filename);

	terrain_map_file >> num_rows >> num_cols;
	terrain_map_file >>
		ws;	 // Extract and discard trailing whitespace (including newline).
	terrain_map_file >> noskipws;  // Don't automatically skip whitespace.
	terrain_map = new TerrainState[num_rows * num_cols];
	display_map = new char[num_rows * num_cols];

	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
			char current_character;
			terrain_map_file >> current_character;
			terrain_map[row * num_cols + col] =
				((current_character == TERRAIN_BLOCKED_CHAR)
					 ? TerrainState::BLOCKED
					 : TerrainState::EMPTY);
		}
		terrain_map_file >> ws;
	}
}

void GameMap::render_and_print_display_map(const Player* const players[],
										   int num_players) const {
	// Render terrain_map onto display_map.
	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
			int index = row * num_cols + col;
			display_map[index] = ((terrain_map[index] == TerrainState::BLOCKED)
									  ? TERRAIN_BLOCKED_CHAR
									  : TERRAIN_EMPTY_CHAR);
		}
	}

	// Render alive units onto display_map.
	for (int p = 0; p < num_players; p += 1) {
		int num_units = players[p]->get_num_units();
		Unit* const* const units = players[p]->get_units();
		for (int u = 0; u < num_units; u += 1) {
			if (units[u]->is_alive()) {
				display_map[units[u]->get_position_row() * num_cols +
							units[u]->get_position_col()] = units[u]->get_id();
			}
		}
	}

	// Print display_map to console.
	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
			cout << display_map[row * num_cols + col];
		}
		cout << '\n';
	}
}

void GameMap::update_terrain_map(int row, int col, TerrainState state) {
	terrain_map[row * num_cols + col] = state;
}

bool GameMap::is_valid_path(int starting_row, int starting_col, int delta_row,
							int delta_col, int movement_range) {
	int end_row = starting_row + delta_row;
	int end_col = starting_col + delta_col;

	return end_row >= 0 && end_row < num_rows	 /*row out-of-bound checking*/
		   && end_col >= 0 && end_col < num_cols /*col out-of-bound checking*/
		   &&
		   terrain_map[end_row * num_cols + end_col] == TerrainState::EMPTY &&
		   manhattan_dist(delta_row, delta_col) <= movement_range;
}
