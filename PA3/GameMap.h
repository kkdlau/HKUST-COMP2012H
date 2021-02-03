#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include <string>
using std::string;

#include "Player.h"
#include "Unit.h"

class GameMap {
   public:
	enum class TerrainState { BLOCKED, EMPTY, OCCUPIED };
	static const char TERRAIN_BLOCKED_CHAR{'#'};
	static const char TERRAIN_EMPTY_CHAR{' '};

	GameMap();
	~GameMap();

	void load_terrain_map(const string& filename);
	void update_terrain_map(int row, int col, TerrainState state);
	bool is_valid_path(int starting_row, int starting_col, int delta_row,
					   int delta_col, int movement_range);
	void render_and_print_display_map(const Player* const players[],
									  int num_players) const;

   private:
	int num_rows, num_cols;
	TerrainState* terrain_map;
	char* display_map;
};

#endif /* GAMEMAP_H_ */
