#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include <string>
using std::string;

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QList>

#include "Player.h"
#include "units/Unit.h"

class GameMap {
public:
	enum class TerrainState { BLOCKED, EMPTY, OCCUPIED };
	static const char TERRAIN_BLOCKED_CHAR{'#'};
	static const char TERRAIN_EMPTY_CHAR{' '};
    static const QString unit_image_lookup[5][4];

	GameMap();
	~GameMap();

	void load_terrain_map(const string& filename);
	void update_terrain_map(int row, int col, TerrainState state);
	bool is_valid_path(int starting_row, int starting_col, int delta_row, int delta_col, int movement_range);
	void render_and_print_display_map(const Player* const players[], int num_players) const;  // OBSOLETE
    void render_map_gui(QGraphicsScene &scene);  // NEW
    void render_units_gui(QGraphicsScene &scene, const Player* const players[], int num_players);  // NEW
    void indicate_area(int row, int col, int rad);  // NEW
    void reset_indicators();  // NEW

private:
	int num_rows, num_cols;
    TerrainState* terrain_map{nullptr};
    char* display_map{nullptr};

    QList<QGraphicsPixmapItem*> map_images;  // NEW
    QList<QGraphicsRectItem*> map_indicators;  // NEW
};

#endif /* GAMEMAP_H_ */
