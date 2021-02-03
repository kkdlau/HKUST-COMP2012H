#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QResizeEvent>
#include "GameMap.h"
#include "Player.h"
#include "units/Unit.h"
#include "units/Swordsman.h"
#include "units/Pikeman.h"
#include "units/Knight.h"
#include "units/Archer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void on_actionMap_triggered();
    void on_actionPlayers_triggered();
    void on_actionStart_triggered();
    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void map_clicked(int row, int col);

    void on_actionHeal_triggered();

private:
    void load_players_and_units(const string& filename);
    int next_player();
    void get_player_unit_by_pos(int row, int col, int &player_out, int &unit_out);
    bool is_game_over() const;

    int num_players{0};
    GameMap game_map;
    Player** players{nullptr};

    QGraphicsScene scene;

    Unit* selected_unit;
    int active_player_id{-1};
    int unit_wait_command{-1};
    int command_stage{0};
    QString available_enemies;
    Ui::GameWindow *ui;

    void resizeEvent(QResizeEvent*) override;
    bool process_unit_move(int row, int col);
    QString search_enemies() const;
    void end_selected_unit_turn();
};
#endif // GAMEWINDOW_H
