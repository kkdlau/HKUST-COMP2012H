#ifndef PA1_H_
#define PA1_H_

const int MAX_ROWS = 25;
const int MAX_COLS = 25;
const int MAX_CELLS = MAX_ROWS * MAX_COLS;

enum Cell {
	HIDDEN = -4,
	FLAGGED = -3,
	SAFE = -2,
	MINE = -1,
	ZERO = 0,
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8
};

// Game State Variables.
extern Cell grid[MAX_ROWS][MAX_COLS];
extern bool has_mine[MAX_ROWS][MAX_COLS];
extern int game_stats[5];  // num_rows, num_cols, num_flags_left,
						   // num_safe_reveals_left, num_incorrect_flags.
extern bool mine_exploded;

void reveal(int row, int col);
void flag(int row, int col);
bool local_solver(int to_reveal[MAX_CELLS][2], int to_flag[MAX_CELLS][2],
				  int num_cells[2]);
void classify_hidden_cells(int constrained_cells[MAX_CELLS][2],
						   int unconstrained_cells[MAX_CELLS][2],
						   int num_hidden_cells[2]);
bool has_satisfiable_neighbor_constraints(int row, int col);
void record_possibility(int constrained_cells[MAX_CELLS][2],
						int num_constrained_cells, int num_flags_bounds_used[4],
						int num_safe_config_records[MAX_CELLS],
						int num_flagged_config_records[MAX_CELLS]);
void enumerate_possibilities(int constrained_cells[MAX_CELLS][2], int index,
							 int num_constrained_cells,
							 int num_flags_bounds_used[4],
							 int num_safe_config_records[MAX_CELLS],
							 int num_flagged_config_records[MAX_CELLS]);
void handle_constrained_cells(int constrained_cells[MAX_CELLS][2],
							  int num_constrained_cells,
							  int num_safe_config_records[MAX_CELLS],
							  int num_flagged_config_records[MAX_CELLS],
							  int cells_to_reveal[MAX_CELLS][2],
							  int cells_to_flag[MAX_CELLS][2],
							  int num_cells[2]);
void handle_unconstrained_cells(int unconstrained_cells[MAX_CELLS][2],
								int num_unconstrained_cells,
								int num_flags_bounds_used[4],
								int cells_to_reveal[MAX_CELLS][2],
								int cells_to_flag[MAX_CELLS][2],
								int num_cells[2]);
bool global_solver(int cells_to_reveal[MAX_CELLS][2],
				   int cells_to_flag[MAX_CELLS][2], int num_cells[2]);

#endif /* PA1_H_ */
