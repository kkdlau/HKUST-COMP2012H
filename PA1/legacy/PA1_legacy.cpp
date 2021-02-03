#include "PA1.h"

#include <iostream>
using namespace std;
/*custom dubug print function*/
#ifdef DEBUG
#define debug_print_cell_msg(row, col, msg) \
	cout << char(col + 'A') << (row + 1) << ": " << msg << endl;
#define debug_print_num(var_alias, value) \
	cout << var_alias << ": " << value << endl;

#define debug_print_cell(arr, length, cells_per_line)             \
	cout << "\n====" << #arr << " start====" << endl;             \
	for (int i = 0; i < length; i++) {                            \
		cout << char(arr[i][1] + 'A') << (arr[i][0] + 1) << ", "; \
		if ((i + 1) % cells_per_line == 0) cout << "\n";          \
	}                                                             \
	cout << "\n====" << #arr << " end====" << endl;
#else
#define debug_print_cell_msg(...)
#define debug_print_num(...)
#define debug_print_cell(...)
#endif
/*custom dubug print function end*/

int& NUM_ROWS = game_stats[0];
int& NUM_COLS = game_stats[1];
int& num_flags_left = game_stats[2];
int& num_safe_reveals_left = game_stats[3];
int& num_incorrect_flags = game_stats[4];

const int MAX_NUM_NEIGHBOR = 8;
bool always_touch_upper = true;
bool always_touch_lower = true;

bool within(const int var, const int minr, const int maxr) {
	return var >= minr && var <= maxr;
}

bool valid_grid(const int row, const int col) {
	return within(row, 0, NUM_ROWS - 1) && within(col, 0, NUM_COLS - 1);
}

#define grid_revealed(r, c) (grid[r][c] >= MINE)
#define _grid (grid[row][col])
#define _mine (has_mine[row][col])
#define is_mine(r, c) has_mine[r][c]
#define is_flag(r, c) (grid[r][c] == FLAGGED)
#define is_hidden(r, c) (grid[r][c] == HIDDEN)
#define int_to_cell_num(ivalue) (static_cast<Cell>(ZERO + ivalue))
#define FOR_EACH_GRID                        \
	for (int row = 0; row < NUM_ROWS; row++) \
		for (int col = 0; col < NUM_COLS; col++)
#define STANDARD_GRID_POS_PARA const int row, const int col
#define cur_grid_pos row, col

#define NEIGHBOR_GRID_TABLE(X, r, c, ...) \
	X(r - 1, c - 1, __VA_ARGS__)          \
	X(r - 1, c, __VA_ARGS__)              \
	X(r - 1, c + 1, __VA_ARGS__)          \
	X(r, c - 1, __VA_ARGS__)              \
	X(r, c + 1, __VA_ARGS__)              \
	X(r + 1, c - 1, __VA_ARGS__)          \
	X(r + 1, c, __VA_ARGS__)              \
	X(r + 1, c + 1, __VA_ARGS__)

#define NEIGHBOR_COUNTER(r, c, ...) +(valid_grid(r, c))
#define NEIGHBOR_MINE_COUNTER(r, c, ...) +((valid_grid(r, c) && is_mine(r, c)))
#define NEIGHBOR_FLAG_COUNTER(r, c, ...) +((valid_grid(r, c) && is_flag(r, c)))
#define NEIGHBOR_HIDDEN_COUNTER(r, c, ...) \
	+((valid_grid(r, c) && is_hidden(r, c)))
#define REVEAL_GRID(r, c, ...) \
	if (valid_grid(r, c)) reveal(r, c);
#define APPEND_UNFLAGGED_GRID(r, c, arr, counter, ...)                \
	if (valid_grid(r, c) && !scanned[r][c] && grid[r][c] == HIDDEN) { \
		arr_2d_insert(arr, counter++, r, c);                          \
		scanned[r][c] = true;                                         \
	}

#define INSERT_NEIGHBOR(r, c, arr, i, ...) \
	if (valid_grid(r, c)) arr_2d_insert(arr, i++, r, c);

#define FOR_EACH_CELL(arr, length)                                \
	for (int i = 0, row = arr[0][0], col = arr[0][1]; i < length; \
		 ++i, row = arr[i][0], col = arr[i][1])

#define SOLVABLE_NEIGHBOR_CHECKER(r, c, ...)                                \
	if (valid_grid(r, c) && grid_revealed(r, c) && !is_solvable_grid(r, c)) \
		return false;

#define FOR_EACH_CONSTRAINED_CELL \
	FOR_EACH_CELL(constrained_cells, num_constrained_cells)

int num_mine_neighbor(STANDARD_GRID_POS_PARA) {
	return (NEIGHBOR_GRID_TABLE(NEIGHBOR_MINE_COUNTER, row, col));
}

int num_flagged_neighbor(STANDARD_GRID_POS_PARA) {
	return (NEIGHBOR_GRID_TABLE(NEIGHBOR_FLAG_COUNTER, row, col));
}

int num_hidden_neighbor(STANDARD_GRID_POS_PARA) {
	return (NEIGHBOR_GRID_TABLE(NEIGHBOR_HIDDEN_COUNTER, row, col));
}

/***********************************************************************
 * TODO 1: Reveal the specified cell.
 * - If the cell is already revealed, do nothing.
 * - If the cell has a mine, display it and set mine_exploded.
 * - If the cell is not a mine, update num_safe_reveals_left in
 *   game_stats[3], and display the number of neighbor mines.
 * - If there are no neighboring mines, unflag and reveal all neighbors.
 * Note: Be careful not to multi-count when updating game_stats[3].
 ***********************************************************************/
void reveal(int row, int col) {
	if (!grid_revealed(row, col)) {
		if (_mine) {
			mine_exploded = true;
			_grid = MINE;
		} else {
			num_safe_reveals_left--;
			int num_mine = num_mine_neighbor(row, col);
			grid[row][col] = int_to_cell_num(num_mine);
			if (!num_mine) {
				NEIGHBOR_GRID_TABLE(REVEAL_GRID, row, col);
			}
		}
	}
}

/***********************************************************************
 * TODO 2: Toggle the flagging of the specified cell.
 * - If the specified cell is hidden, flag it, and vice versa.
 * - Update num_flags_left in game_stats[2].
 * - Update num_incorrect_flags in game_stats[4].
 ***********************************************************************/
void flag(int row, int col) {
	if (!grid_revealed(row, col)) {
		if (_grid != FLAGGED) {
			_grid = FLAGGED;
			num_flags_left--;
			num_incorrect_flags += !is_mine(row, col);
		} else {
			_grid = HIDDEN;
			num_flags_left++;
			num_incorrect_flags -= !is_mine(row, col);
		}
	}
}

void arr_2d_insert(int arr[][2], const int index, int e1, int e2) {
	arr[index][0] = e1;
	arr[index][1] = e2;
}

/***********************************************************************
 * TODO 3: Determine hidden cells to reveal and to flag, considering
 * only one constraint at a time.
 * - If the number of flagged cells surrounding a constraint is equal to
 *   the constraint, then reveal all surrounding unflagged hidden cells.
 * - If the number of hidden + flagged cells surrounding a constraint is equal
 *to the constraint, then flag all surrounding unflagged hidden cells.
 * - Return whether or not these two rules are successful in
 *   determining at least one cell to reveal or to flag.
 ***********************************************************************/

bool local_solver(int cells_to_reveal[MAX_CELLS][2],
				  int cells_to_flag[MAX_CELLS][2], int num_cells[2]) {
	int& num_to_reveal = num_cells[0] = 0;
	int& num_to_flag = num_cells[1] = 0;
	num_to_flag = 0;
	bool scanned[MAX_ROWS][MAX_COLS] = {0};

	FOR_EACH_GRID {
		const int num_flag = num_flagged_neighbor(row, col);
		const int num_hidden = num_hidden_neighbor(row, col);

		if (_grid > ZERO) {
			if (_grid == num_flag) {
				NEIGHBOR_GRID_TABLE(APPEND_UNFLAGGED_GRID, row, col,
									cells_to_reveal, num_to_reveal);
			} else if (_grid == num_flag + num_hidden) {
				NEIGHBOR_GRID_TABLE(APPEND_UNFLAGGED_GRID, row, col,
									cells_to_flag, num_to_flag);
			}
		}
	}
	return num_to_reveal > 0 || num_to_flag > 0;
}

/***********************************************************************
 * TODO 4: Record all unflagged hidden cells and classify each as
 * either constrained or unconstrained.
 * - If an unflagged hidden cell has a neighboring constraint, record
 *   it as constrained.
 * - If an unflagged hidden cell has no neighboring constraints, record
 *   it as unconstrained.
 ***********************************************************************/
#define FOR_EACH_NEIGHBOR(r, c)                                    \
	int neighbors[MAX_NUM_NEIGHBOR][2] = {0};                      \
	int length = 0;                                                \
	NEIGHBOR_GRID_TABLE(INSERT_NEIGHBOR, r, c, neighbors, length); \
	FOR_EACH_CELL(neighbors, length)

bool has_constrainted_neighbor(STANDARD_GRID_POS_PARA) {
	FOR_EACH_NEIGHBOR(row, col) {
		if (_grid > ZERO) return true;
	}

	return false;
}

void classify_hidden_cells(int constrained_cells[MAX_CELLS][2],
						   int unconstrained_cells[MAX_CELLS][2],
						   int num_cells[2]) {
	num_cells[0] = num_cells[1] = 0;
	FOR_EACH_GRID {
		if (_grid == HIDDEN) {
			if (has_constrainted_neighbor(cur_grid_pos)) {
				arr_2d_insert(constrained_cells, num_cells[0]++, row, col);
			} else {
				arr_2d_insert(unconstrained_cells, num_cells[1]++, row, col);
			}
		}
	}
}

/***********************************************************************
 * TODO 5: Check whether or not all neighboring constraints of the
 * specified cell can still be satisfied.
 * - Return true if there exists a possible configuration of mines
 *   which agrees with all neighboring constraints, without considering
 *   the total number of mines in the grid. Otherwise, return false.
 *
 * Usage: enumerate_possibilities() will test-set the specified cell as
 * FLAGGED/SAFE, and then call this function to check for validity.
 * Hint: For each of the specified cell (A)'s neighbors (N1 - N8), if Ni
 * is revealed with a number, count Ni's neighboring hidden and flagged
 * cells (one of them is A itself) and check if it is still solvable.
 ***********************************************************************/
bool is_solvable_grid(STANDARD_GRID_POS_PARA) {
	const int num_flag = num_flagged_neighbor(cur_grid_pos);
	const int num_hidden = num_hidden_neighbor(cur_grid_pos);
	if (_grid < num_flag) return false;

	return _grid <= num_flag + num_hidden;
}

bool has_satisfiable_neighbor_constraints(int row, int col) {
	// debug_print_cell_msg(row, col, "checking
	// satisfiable_neighbor_constraints");
	NEIGHBOR_GRID_TABLE(SOLVABLE_NEIGHBOR_CHECKER, row, col);
	return true;
}

/***********************************************************************
 * TODO 6: Validate and record the current configuration of the
 * constrained cells.
 * - Check if the number of flags used in the current configuration of
 *   the constrained cells is within the lower and upper bounds.
 * - If within, record the SAFE/FLAGGED states of the constrained cells,
 *   and update the lowest and the highest number of flags used across
 *   all recorded configurations so far.
 ***********************************************************************/
#define lower_bound (num_flags_bounds_used[0])
#define upper_bound (num_flags_bounds_used[1])
#define lowest_num_flag (num_flags_bounds_used[2])
#define highest_num_flag (num_flags_bounds_used[3])
void record_possibility(int constrained_cells[MAX_CELLS][2],
						int num_constrained_cells, int num_flags_bounds_used[4],
						int num_safe_config_records[MAX_CELLS],
						int num_flagged_config_records[MAX_CELLS]) {
	int used_flag = 0;

	FOR_EACH_CONSTRAINED_CELL {
		if (_grid == FLAGGED) {
			used_flag++;
			num_flagged_config_records[i]++;
		} else if (_grid == SAFE) {
			num_safe_config_records[i]++;
		}
	}
	debug_print_num("save - used flag", used_flag);
	if (within(used_flag, lower_bound, upper_bound)) {
		if (used_flag < lowest_num_flag) lowest_num_flag = used_flag;
		if (used_flag > highest_num_flag) highest_num_flag = used_flag;
		if (used_flag != lower_bound) always_touch_lower = false;
		if (used_flag != upper_bound) always_touch_upper = false;
	} else {
		FOR_EACH_CONSTRAINED_CELL {
			if (_grid == FLAGGED) {
				num_flagged_config_records[i]--;
			} else if (_grid == SAFE) {
				num_safe_config_records[i]--;
			}
		}
	}
}

/***********************************************************************
 * TODO 7: Enumerate, validate, and record all possible configurations
 * of the constrained cells.
 * - Enumerate through all possibilities of the constrained cells by
 *   setting them one-by-one to FLAGGED/SAFE in the grid, then checking
 *   for validity. Use index to keep track of progress.
 * - Stop and backtrack from the exploration of a configuration, if
 *   setting the current constrained cell as FLAGGED/SAFE causes it to
 *   have unsatisfiable neighbor constraints.
 * - If the exploration of a configuration reaches the end, (i.e. all
 *   constrained cells successfully set to FLAGGED/SAFE while satisfying
 *   all neighboring constraints), validate the number of flags used and
 *   record this configuration of the constrained cells.
 * - Remember to reset the constrained cells back to HIDDEN in the grid
 *   when finished with a configuration.
 *
 * Warning: This function potentially has exponential runtime, as the
 * number of possible configurations is 2^num_constrained if not
 * checking validity via satisfiable neighbor constraints.
 * Note: Good usage of recursion will greatly reduce the amount of
 * coding required to implement this function (The TA solution is
 * shorter than this TODO comment block).
 *
 * Hint: You may draw a "binary branching tree graph" to help you
 * visualize all the configurations as well as the recursion pathways.
 * Example enumeration with 4 constrained cells (2^4 = 16 configurations):
 * START ----> SAFE -----> SAFE -----> SAFE -----> SAFE
 *         |           |           |           |
 *         |           |           |           --> FLAGGED
 *         |           |           |
 *         |           |           --> FLAGGED --> SAFE
 *         |           |                       |
 *         |           |                       --> FLAGGED
 *         |           |
 *         |           --> FLAGGED --> SAFE -----> SAFE
 *         |                       |           |
 *         |                       |           --> FLAGGED
 *         |                       |
 *         |                       --> FLAGGED --> SAFE
 *         |                                   |
 *         |                                   --> FLAGGED
 *         |
 *         --> FLAGGED --> SAFE -----> SAFE -----> SAFE
 *                     |           |           |
 *                     |           |           --> FLAGGED
 *                     |           |
 *                     |           --> FLAGGED --> SAFE
 *                     |                       |
 *                     |                       --> FLAGGED
 *                     |
 *                     --> FLAGGED --> SAFE -----> SAFE
 *                                 |           |
 *                                 |           --> FLAGGED
 *                                 |
 *                                 --> FLAGGED --> SAFE
 *                                             |
 *                                             --> FLAGGED
 ***********************************************************************/

#define last_index (num_constrained_cells - 1 == index)
void enumerate_possibilities(int constrained_cells[MAX_CELLS][2], int index,
							 int num_constrained_cells,
							 int num_flags_bounds_used[4],
							 int num_safe_config_records[MAX_CELLS],
							 int num_flagged_config_records[MAX_CELLS]) {
	int row = constrained_cells[index][0];
	int col = constrained_cells[index][1];
	debug_print_cell_msg(row, col, "enuming, index " << index);

	_grid = SAFE;
	bool satisfy_constraint = has_satisfiable_neighbor_constraints(row, col);
	debug_print_num("\tSAFE - satisfy_constraint", satisfy_constraint);

	if (last_index && satisfy_constraint) {
		record_possibility(constrained_cells, num_constrained_cells,
						   num_flags_bounds_used, num_safe_config_records,
						   num_flagged_config_records);
	} else if (satisfy_constraint) {
		enumerate_possibilities(constrained_cells, index + 1,
								num_constrained_cells, num_flags_bounds_used,
								num_safe_config_records,
								num_flagged_config_records);
	}
	_grid = FLAGGED;
	satisfy_constraint = has_satisfiable_neighbor_constraints(row, col);
	debug_print_num("\tFLAGGED - satisfy_constraint", satisfy_constraint);

	if (last_index && satisfy_constraint) {
		record_possibility(constrained_cells, num_constrained_cells,
						   num_flags_bounds_used, num_safe_config_records,
						   num_flagged_config_records);
	} else if (satisfy_constraint) {
		enumerate_possibilities(constrained_cells, index + 1,
								num_constrained_cells, num_flags_bounds_used,
								num_safe_config_records,
								num_flagged_config_records);
	}

	_grid = HIDDEN;
}

/***********************************************************************
 * TODO 8: Determine which constrained cells must be revealed or flagged.
 * - If a constrained cell has always been recorded as SAFE, reveal it.
 * - If a constrained cell has always been recorded as FLAGGED, flag it.
 ***********************************************************************/
#define always_safe \
	(num_safe_config_records[i] && !num_flagged_config_records[i])
#define always_flagged \
	(num_flagged_config_records[i] && !num_safe_config_records[i])

void handle_constrained_cells(int constrained_cells[MAX_CELLS][2],
							  int num_constrained_cells,
							  int num_safe_config_records[MAX_CELLS],
							  int num_flagged_config_records[MAX_CELLS],
							  int cells_to_reveal[MAX_CELLS][2],
							  int cells_to_flag[MAX_CELLS][2],
							  int num_cells[2]) {
	int& num_to_reveal = num_cells[0] = 0;
	int& num_to_flag = num_cells[1] = 0;

	FOR_EACH_CONSTRAINED_CELL {
		debug_print_cell_msg(row, col,
							 boolalpha << "always safe: " << always_safe
									   << ", always_flagged:" << always_flagged
									   << ". num_safe | num_flagged: "
									   << num_safe_config_records[i] << " | "
									   << num_flagged_config_records[i]);
		if (always_safe) {
			arr_2d_insert(cells_to_reveal, num_to_reveal++, row, col);
		} else if (always_flagged) {
			arr_2d_insert(cells_to_flag, num_to_flag++, row, col);
		}
	}
}

/***********************************************************************
 * TODO 9: Determine if all unconstrained cells must be revealed or
 *flagged.
 * - If the number of flags used in all configurations of the
 *   constrained cells is always equal to the upper bound, then all
 *   unconstrained cells must be revealed.
 * - If the number of flags used in all configurations of the
 *   constrained cells is always equal to the lower bound, then all
 *   unconstrained cells must be flagged.
 ***********************************************************************/
#define FOR_EACH_UNCONSTRAINED_CELL \
	FOR_EACH_CELL(unconstrained_cells, num_unconstrained_cells)

void handle_unconstrained_cells(int unconstrained_cells[MAX_CELLS][2],
								int num_unconstrained_cells,
								int num_flags_bounds_used[4],
								int cells_to_reveal[MAX_CELLS][2],
								int cells_to_flag[MAX_CELLS][2],
								int num_cells[2]) {
	int& num_to_reveal = num_cells[0];
	int& num_to_flag = num_cells[1];

	if (always_touch_upper) {
		FOR_EACH_UNCONSTRAINED_CELL {
			arr_2d_insert(cells_to_reveal, num_to_reveal++, row, col);
		}
	} else if (always_touch_lower) {
		FOR_EACH_UNCONSTRAINED_CELL {
			arr_2d_insert(cells_to_flag, num_to_flag++, row, col);
		}
	}
}

/***********************************************************************
 * TODO 10: Determine hidden cells to reveal and to flag,
 *considering all constraints simultaneously.
 * - Record all unflagged hidden cells and classify each as either
 *   constrained or unconstrained.
 * - Calculate the upper and lower bounds on the number of flags
 *used in the configurations of the constrained cells. Upper bound
 *assumes all unconstrained cells are safe, while lower bound
 *assumes all unconstrained cells are mines.
 * - Enumerate and record all possible configurations of the
 *constrained cells and number of flags used.
 * - Determine which constrained cells must be revealed or flagged.
 * - Determine if all unconstrained cells must be revealed or
 *flagged.
 * - Return whether or not this strategy is successful in
 *determining at least one cell to reveal or to flag.
 ***********************************************************************/
int num_unflagged_mines() {
	int count = 0;
	FOR_EACH_GRID {
		if (_grid == HIDDEN && _mine) count++;
	}

	return count;
}

bool global_solver(int cells_to_reveal[MAX_CELLS][2],
				   int cells_to_flag[MAX_CELLS][2], int num_cells[2]) {
	int& num_to_reveal = num_cells[0] = 0;
	int& num_to_flag = num_cells[1] = 0;

	always_touch_upper = true;
	always_touch_lower = true;
	int constrained_cells[MAX_CELLS][2];
	int unconstrained_cells[MAX_CELLS][2];
	int cell_type_index[2];
	classify_hidden_cells(constrained_cells, unconstrained_cells,
						  cell_type_index);
	debug_print_cell(constrained_cells, cell_type_index[0], 20);
	debug_print_cell(unconstrained_cells, cell_type_index[1], 20);
	int num_flags_bounds_used[4] = {0, 0, MAX_CELLS, -MAX_CELLS};
	int num_mine = num_unflagged_mines();

	upper_bound = num_mine;
	lower_bound = num_mine - cell_type_index[1];
	debug_print_num("upper_bound", upper_bound);
	debug_print_num("lower_bound", lower_bound);

	int num_safe_config_records[MAX_CELLS] = {0};
	int num_flagged_config_records[MAX_CELLS] = {0};
	if (cell_type_index[0])
		enumerate_possibilities(constrained_cells, 0, cell_type_index[0],
								num_flags_bounds_used, num_safe_config_records,
								num_flagged_config_records);

	handle_constrained_cells(
		constrained_cells, cell_type_index[0], num_safe_config_records,
		num_flagged_config_records, cells_to_reveal, cells_to_flag, num_cells);

	handle_unconstrained_cells(unconstrained_cells, cell_type_index[1],
							   num_flags_bounds_used, cells_to_reveal,
							   cells_to_flag, num_cells);

	return num_to_reveal || num_to_flag;
}