#include <fstream>
using std::ifstream;
using std::noskipws;
using std::ws;
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::abs;

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QDebug>
#include <QtMath>
#include "GameMap.h"

const QString GameMap::unit_image_lookup[5][4] {
    {
        ":/images/unit_images/infantry_red.png",
        ":/images/unit_images/pikeman_red.png",
        ":/images/unit_images/cavalry_red.png",
        ":/images/unit_images/archer_red.png"
    },
    {
        ":/images/unit_images/infantry_blue.png",
        ":/images/unit_images/pikeman_blue.png",
        ":/images/unit_images/cavalry_blue.png",
        ":/images/unit_images/archer_blue.png"
    },
    {
        ":/images/unit_images/infantry_gray.png",
        ":/images/unit_images/pikeman_gray.png",
        ":/images/unit_images/cavalry_gray.png",
        ":/images/unit_images/archer_gray.png"
    },
    {
        ":/images/unit_images/infantry_green.png",
        ":/images/unit_images/pikeman_green.png",
        ":/images/unit_images/cavalry_green.png",
        ":/images/unit_images/archer_green.png"
    },
    {
        ":/images/unit_images/infantry_yellow.png",
        ":/images/unit_images/pikeman_yellow.png",
        ":/images/unit_images/cavalry_yellow.png",
        ":/images/unit_images/archer_yellow.png"
    }
};

GameMap::GameMap() {}
GameMap::~GameMap() {
	delete[] terrain_map;
	delete[] display_map;
}

void GameMap::load_terrain_map(const string& filename) {
	ifstream terrain_map_file(filename);

	terrain_map_file >> num_rows >> num_cols;
	terrain_map_file >> ws; // Extract and discard trailing whitespace (including newline).
	terrain_map_file >> noskipws; // Don't automatically skip whitespace.
    delete[] terrain_map;
    delete[] display_map;
	terrain_map = new TerrainState[num_rows * num_cols];
	display_map = new char[num_rows * num_cols];

	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
			char current_character;
			terrain_map_file >> current_character;
			terrain_map[row * num_cols + col] = ((current_character == TERRAIN_BLOCKED_CHAR) ? TerrainState::BLOCKED : TerrainState::EMPTY);
		}
		terrain_map_file >> ws;
	}
}

void GameMap::update_terrain_map(int row, int col, TerrainState state) { terrain_map[row * num_cols + col] = state; }

bool GameMap::is_valid_path(int starting_row, int starting_col, int delta_row, int delta_col, int movement_range) {
	int distance = abs(delta_row) + abs(delta_col);
	if (distance > movement_range) { return false; }

	int destination_row = starting_row + delta_row;
	int destination_col = starting_col + delta_col;
	if (destination_row < 0 || destination_row >= num_rows || destination_col < 0 || destination_col >= num_cols) { return false; }

	int destination_index = destination_row * num_cols + destination_col;
	if ((terrain_map[destination_index] == TerrainState::BLOCKED) || (terrain_map[destination_index] == TerrainState::OCCUPIED)) { return false; }

	return true;
}

void GameMap::render_and_print_display_map(const Player* const players[], int num_players) const {
    // Old function from PA3, should be useless, just for reference. 
	// Render terrain_map onto display_map.
	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
			int index = row * num_cols + col;
			display_map[index] = ((terrain_map[index] == TerrainState::BLOCKED) ? TERRAIN_BLOCKED_CHAR : TERRAIN_EMPTY_CHAR);
		}
	}

	// Render alive units onto display_map.
	for (int p = 0; p < num_players; p += 1) {
		int num_units = players[p]->get_num_units();
		Unit* const* const units = players[p]->get_units();
		for (int u = 0; u < num_units; u += 1) {
			if (units[u]->is_alive()) { display_map[units[u]->get_position_row() * num_cols + units[u]->get_position_col()] = units[u]->get_id(); }
		}
	}

	// Print display_map to console.
	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_cols; col += 1) {
            cout << display_map[row * num_cols + col];
		}
        cout << endl;
	}
}

void GameMap::render_map_gui(QGraphicsScene &scene) {
    // TODO: instead of writing display_map[], draw the game map in the upper panel (QGraphicsScene and QGraphicsView)
    //       You can consider to create the square indicators beforehand and set them to invisible. 
    // Add an image to the scene: QGraphicsPixmapItem *pix_item = scene.addPixmap(QPixmap(imagename));
    // Change the location: pix_item->setOffset(col*16, row*16); (image size is 16x16)
    // useful docs: https://doc.qt.io/qt-5/qgraphicsscene.html#addPixmap
    // https://doc.qt.io/qt-5/qpixmap.html
    // https://doc.qt.io/qt-5/qgraphicspixmapitem.html#setOffset
    // set the top/bottom of the images: https://doc.qt.io/qt-5/qgraphicsitem.html#setZValue
    // https://doc.qt.io/qt-5/qgraphicsitem.html#setVisible
    // https://doc.qt.io/qt-5/qgraphicsscene.html#addRect
    // zoom the scene to fill the panel: https://doc.qt.io/qt-5/qgraphicsview.html#fitInView
    scene.clear();
    map_indicators.clear();

    for (int row = 0; row < num_rows; row += 1) {
        for (int col = 0; col < num_cols; col += 1) {
            QGraphicsPixmapItem* tiled_grid;
            if (terrain_map[row * num_cols + col] == TerrainState::BLOCKED)
                tiled_grid = new QGraphicsPixmapItem{QPixmap{":/images/terrain_images/mountain.png"}};
            else
                tiled_grid = new QGraphicsPixmapItem{QPixmap{":/images/terrain_images/plains.png"}};
            map_images.push_back(tiled_grid);
            tiled_grid->setOffset(col * 16, row * 16);
            scene.addItem(tiled_grid);

            QGraphicsRectItem* indicate = new QGraphicsRectItem{col * 16.0f, row * 16.0f, 16.0f, 16.0f};
            indicate->setZValue(1);
            map_indicators.push_back(indicate);
            indicate->setVisible(false);
            scene.addItem(indicate);

        }
    }
}

void GameMap::render_units_gui(QGraphicsScene &scene, const Player* const players[], int num_players) {
    for (int p = 0; p < num_players; ++p) {
        const Player* current_player = players[p];
        const int num_units = current_player->get_num_units();
        for (int u = 0; u < num_units; ++u) {
            Unit* current_unit = current_player->get_units()[u];
            QGraphicsPixmapItem* view = new QGraphicsPixmapItem{QPixmap{unit_image_lookup[p][static_cast<int>(current_unit->get_unit_type())]}};
            current_unit->register_view(view);
            scene.addItem(view);
        }

    }
}

void GameMap::reset_indicators() {
    // TODO: make all the indicators invisible
    QList<QGraphicsRectItem*>::Iterator ptr;

    for (ptr = map_indicators.begin(); ptr < map_indicators.end(); ++ptr) {
        (*ptr)->setVisible(false);
    }
}

void GameMap::indicate_area(int row, int col, int rad) {
    for (int r = 0; r < num_rows; ++r) {
        for (int c = 0; c < num_cols; ++c) {
            if (abs(r - row) + abs(c - col) <= rad)
                map_indicators[r * num_cols + c]->setVisible(true);
        }
    }
}
