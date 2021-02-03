#include <fstream>

#include <QFileDialog>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QtMath>
#include <iostream>
#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "clickableview.h"
using namespace std;

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    // Link the scene with the view
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->show();

    // connect the signal of the click event in the upper panel and the handler in this class
    connect(ui->graphicsView, &ClickableView::mouseClicked, this, &GameWindow::map_clicked);

    // hide buttons that should not be pressed now
    ui->actionPlayers->setVisible(false);
    ui->actionStart->setVisible(false);
    ui->actionHeal->setVisible(false);
    selected_unit = nullptr;

//    on_actionMap_triggered();
//    on_actionPlayers_triggered();
//    on_actionStart_triggered();
}

GameWindow::~GameWindow()
{
    delete ui;
    for (int i=0; i<num_players; i++) delete players[i];
    delete[] players;
}

void GameWindow::resizeEvent(QResizeEvent* e) {
    emit QMainWindow::resizeEvent(e);
//    ui->graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
}

void GameWindow::on_actionMap_triggered()
{

    // FIXME: change on_actionMap_triggered to ask file

//    QString filename = QFileDialog::getOpenFileName(this, tr("Open Map File"), ".", tr("Text Files (*.txt)"));
//    qDebug() << filename;  // You can use qDebug() for debug info

    QString filename = "/Users/dannylau/Program/COMP2012H/PA4/GameMap_sd.txt";
    if (filename == "") return;
    for (int p = 0; p < num_players; ++p) {
        delete players[p];
    }
    delete [] players;
    players = nullptr;
    num_players = 0;

    game_map.load_terrain_map(filename.toUtf8().constData());
    game_map.render_map_gui(scene);
    ui->graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
    ui->actionPlayers->setVisible(true);
    ui->actionStart->setVisible(false);

}

void GameWindow::on_actionPlayers_triggered()
{
    // The handler after the Players button is pressed
    // TODO: load player and unit file and render the UI

    QString filename = "/Users/dannylau/Program/COMP2012H/PA4/PlayersAndUnits_sd.txt";
//    QString filename = QFileDialog::getOpenFileName(this, tr("Open Map File"), ".", tr("Text Files (*.txt)"));
    qDebug() << filename;
    if (filename == "") return;
    load_players_and_units(filename.toUtf8().constData());
    game_map.render_units_gui(scene, players, num_players);
    ui->actionStart->setVisible(true);
}

void GameWindow::load_players_and_units(const string& filename) {
    for (int p = 0; p < num_players; ++p) {
        delete players[p];
    }
    delete [] players;
    players = nullptr;
    num_players = 0;

    ifstream players_and_units_file(filename);
    players_and_units_file >> num_players;

    qDebug() << "load_players_and_units:" << num_players;
    players = new Player*[num_players];

    for (int p = 0; p < num_players; p += 1) {
        string name;

        int num_units;
        players_and_units_file >> name >> num_units;
        QString qname{name.c_str()};
        qname = qname.toLower();

        qDebug() << name.c_str();
        QTreeWidgetItem * team_panel = new QTreeWidgetItem(ui->treeWidget);
        ui->treeWidget->addTopLevelItem(team_panel);
        team_panel->setText(0, name.c_str());
        Unit** units = new Unit*[num_units];

        for (int u = 0; u < num_units; u += 1) {
            char id;
            string unit_type_string;
            int row, col;
            players_and_units_file >> id >> unit_type_string >> row >> col;
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
            QTreeWidgetItem* status = new QTreeWidgetItem{};
            team_panel->addChild(status);
            unit->register_list_item(status);
            units[u] = unit;

            //cout << unit->to_string() << endl;
            game_map.update_terrain_map(row, col, GameMap::TerrainState::OCCUPIED);
        }
        players[p] = new Player{name, num_units, units};
        players[p]->register_list_item(team_panel);
    }

}

int GameWindow::next_player() {
    return (active_player_id + 1) % num_players;
}

void GameWindow::get_player_unit_by_pos(int row, int col, int &player_out, int &unit_out) {
    qDebug() << "start get_player_unit_by_pos";
    qDebug() << "num_players: " << num_players;
    for (int p = 0; p < num_players; ++p) {
        const int num_units = players[p]->get_num_units();
        qDebug() << "checking: " << p << " num_units:" << num_units;
        for (int u = 0; u < num_units; ++u) {
            Unit* to_check = players[p]->get_units()[u];
            qDebug() << "checking: " << to_check->to_string().c_str();
            if (to_check->is_alive() && row == to_check->get_position_row() && col == to_check->get_position_col()) {
                player_out = p;
                unit_out = u;
                return;
            }
        }
    }
}

void GameWindow::on_actionStart_triggered()
{
    ui->actionPlayers->setVisible(false);
    ui->actionStart->setVisible(false);
    ui->actionMap->setVisible(false);
    active_player_id = next_player();
    players[active_player_id]->ready_all_units();
}

void GameWindow::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if (is_game_over()) return;
    bool is_active_player_units = ui->treeWidget->indexOfTopLevelItem(item->parent()) == active_player_id;
    qDebug() << "is_active_player_units: " << is_active_player_units;
    if (item->parent() && is_active_player_units) {
        int index = item->parent()->indexOfChild(item);
        Unit* u = players[active_player_id]->get_units()[index];
        qDebug() << "command_stage:" << command_stage << "u->is_alive(): " << u->is_alive() << "u->is_ready():" << u->is_ready();
        if (command_stage <= 1 && u->is_alive() && u->is_ready()) {
            selected_unit = u;
            unit_wait_command = -1;
            command_stage = 0;
            map_clicked(selected_unit->get_position_row(), selected_unit->get_position_col());
        }
    }
}

void GameWindow::map_clicked(int row, int col) {
    if (is_game_over()) return;
    int selected_player = -1, unit_index;
    get_player_unit_by_pos(row, col, selected_player, unit_index);
    qDebug() << "get_player_unit_by_pos ok";
    Unit* u = nullptr;
    if (selected_player != -1)
        u = players[selected_player]->get_units()[unit_index];

    if (active_player_id != -1) {
        if (unit_wait_command == -1) {
            switch (command_stage) {
            case 0: { // normal
                if (selected_player == active_player_id && u->is_ready()) {
                    selected_unit = u;
                    qDebug() << "selected_player == active_player_id && u->is_ready() ok";
                    game_map.reset_indicators();
                    qDebug() << "reset!";
                    game_map.indicate_area(selected_unit->get_position_row(), selected_unit->get_position_col(), selected_unit->get_movement_range());
                } else break;
            }
            case 1: { // move
                qDebug() << "UPDATE";
                ui->actionHeal->setVisible(true);
                game_map.reset_indicators();
                game_map.indicate_area(selected_unit->get_position_row(), selected_unit->get_position_col(), selected_unit->get_movement_range());
                unit_wait_command = 0;
                command_stage = 1;
            } break;
            case 2: { //attack
                break;
            }};
        } else {
            switch (command_stage) {
            case 0: { // normal
                break;
            }
            case 1: { // move
                bool success = process_unit_move(row, col);
                if (success) {
                    ui->actionHeal->setVisible(false);
                    game_map.reset_indicators();
                    available_enemies = search_enemies();
                    if (available_enemies != "") {
                        game_map.indicate_area(selected_unit->get_position_row(), selected_unit->get_position_col(), selected_unit->get_attack_range());
                        command_stage = 2;
                        qDebug() << "Enter attack phase";
                    } else end_selected_unit_turn();
                }
            } break;
            case 2: { //attack
                if (selected_unit->get_position_row() == row && selected_unit->get_position_col() == col) end_selected_unit_turn();

                if (u && available_enemies.contains(u->get_id())) {
                    selected_unit->attack_unit(u);
                    if (!selected_unit->is_alive()) { game_map.update_terrain_map(selected_unit->get_position_row(), selected_unit->get_position_col(), GameMap::TerrainState::EMPTY); }
                    if (!u->is_alive()) { game_map.update_terrain_map(u->get_position_row(), u->get_position_col(), GameMap::TerrainState::EMPTY); }
                    game_map.reset_indicators();
                    end_selected_unit_turn();
                }
                break;
            }};
        }
    }
}

bool GameWindow::is_game_over() const {
    int num_players_remaining = 0;
    for (int i = 0; i < num_players; i += 1) {
        if (players[i]->has_units_alive()) { num_players_remaining += 1; }
        if (num_players_remaining > 1) { return false; }
    }
    return true;
}

void GameWindow::on_actionHeal_triggered()
{
    if (selected_unit) {
        selected_unit->heal();
        game_map.reset_indicators();
        end_selected_unit_turn();
    }
}

bool GameWindow::process_unit_move(int row, int col) {
    int starting_row = selected_unit->get_position_row();
    int starting_col = selected_unit->get_position_col();
    if (starting_col == col && starting_row == row) return true;
    int delta_row = row - starting_row;
    int delta_col = col - starting_col;
    if (game_map.is_valid_path(starting_row, starting_col, delta_row, delta_col, selected_unit->get_movement_range())) {
        selected_unit->move_delta(delta_row, delta_col);
        game_map.update_terrain_map(starting_row, starting_col, GameMap::TerrainState::EMPTY);
        game_map.update_terrain_map(starting_row + delta_row, starting_col + delta_col, GameMap::TerrainState::OCCUPIED);
        return true;
    } else return false;
}

QString GameWindow::search_enemies() const {
    QString enemy_units_in_range_id{""};
    for (int p = 0; p < num_players; p += 1) {
        if (p == active_player_id) { continue; }
        for (int u = 0; u < players[p]->get_num_units(); u += 1) {
            Unit* enemy_unit = players[p]->get_units()[u];
            if (enemy_unit->is_alive()) {
                int distance_row = abs(selected_unit->get_position_row() - enemy_unit->get_position_row());
                int distance_col = abs(selected_unit->get_position_col() - enemy_unit->get_position_col());
                if ((distance_row + distance_col) <= selected_unit->get_attack_range()) { enemy_units_in_range_id += enemy_unit->get_id(); }
            }
        }
    }

    return enemy_units_in_range_id;
}

void GameWindow::end_selected_unit_turn() {
    selected_unit->end_turn();
    game_map.reset_indicators();
    unit_wait_command = -1;
    command_stage = 0;
    ui->actionHeal->setVisible(false);
    if (is_game_over()) {
        QMessageBox::information(this,"Info", "Game End");
    } else if (!players[active_player_id]->has_units_ready()) {
        active_player_id = next_player();
        qDebug() << "active_player_id: " << active_player_id;
        players[active_player_id]->ready_all_units();
    }
}
