#include <iostream>
using namespace std;

#include "PA1.h"

// Our own simple implementation of a pseudo-random number generator, for
// debugging and automated grading consistency.
static unsigned long rng_lab2 = 0;

static void _srand(unsigned seed) { rng_lab2 = seed; }

static int _rand() {
	rng_lab2 = (1103515245 * rng_lab2 + 12345) % (1 << 31);
	return rng_lab2 & 0x7FFFFFFF;
}

// Game State Variables.
Cell grid[MAX_ROWS][MAX_COLS];
bool has_mine[MAX_ROWS][MAX_COLS];	// Default value of false.
int game_stats[5];					// num_rows, num_cols, num_flags_left,
					// num_safe_reveals_left, num_incorrect_flags.
bool mine_exploded = false;

// Top-Level Gameplay Functions.
void generate_grid() {
	cout << "Please input the following:" << '\n';

	do {
		cout << "Number of rows (5-25): ";
		cin >> game_stats[0];
	} while (game_stats[0] < 5 || game_stats[0] > 25);

	do {
		cout << "Number of cols (5-25): ";
		cin >> game_stats[1];
	} while (game_stats[1] < 5 || game_stats[1] > 25);

	for (int row = 0; row < game_stats[0]; row += 1) {
		for (int col = 0; col < game_stats[1]; col += 1) {
			grid[row][col] = HIDDEN;
		}
	}

	do {
		cout << "Number of mines (1 mine - 20% mines): ";
		cin >> game_stats[2];
	} while ((game_stats[2] < 1) ||
			 (5 * game_stats[2] > game_stats[0] * game_stats[1]));
	game_stats[3] = (game_stats[0] * game_stats[1]) - game_stats[2];
	game_stats[4] = 0;

	// Pseudo-random Mine Placement.
	cout << "Game ID (0-100): ";
	unsigned int id;
	cin >> id;
	_srand(id + 2012);

	for (int n = 0; n < game_stats[2]; n += 1) {
		//		// Potential infinite loop if PRNG sequence after modulo has a
		// number range that is too narrow. 		int row, col; 		do {
		// row
		// = _rand() % game_stats[0]; 			col = _rand() % game_stats[1]; }
		// while (has_mine[row][col]); 		has_mine[row][col] = true;

		int row = _rand() % game_stats[0];
		int col = _rand() % game_stats[1];

		// If already have a mine in this location, increment col and
		// wrap-around in row-major order.
		while (has_mine[row][col]) {
			row = (row + ((col + 1) / game_stats[1])) % game_stats[0];
			col = (col + 1) % game_stats[1];
		}
		has_mine[row][col] = true;
	}
}

void display_grid() {
	cout << '\n';
	cout << "  ";
	for (int col = 0; col < game_stats[1]; col += 1) {
		cout << ' ' << char(col + 'A');
	}
	cout << '\n';

	for (int row = 0; row < game_stats[0]; row += 1) {
		if (row < 9) {
			cout << ' ';
		}
		cout << row + 1;

		for (int col = 0; col < game_stats[1]; col += 1) {
			cout << ' ';
			switch (grid[row][col]) {
				case HIDDEN:
					cout << 'o';
					break;
				case FLAGGED:
					cout << 'x';
					break;
				case SAFE:
					cout << '?';
					break;
				case MINE:
					cout << '!';
					break;
				case ZERO:
					cout << '.';
					break;
				default:
					cout << grid[row][col];	 // Unscoped enum implicit
											 // conversion to int.
			}
		}
		cout << '\n';
	}

	cout << '\n';
	cout << "Flags Left: " << game_stats[2] << '\n';
	cout << "Safe cells left: " << game_stats[3] << '\n';
	cout << "Wrong flag: " << game_stats[4] << '\n';
	cout << '\n';
}

void display_answer() {
	if (mine_exploded) {
		for (int row = 0; row < game_stats[0]; row += 1) {
			for (int col = 0; col < game_stats[1]; col += 1) {
				if (has_mine[row][col] && grid[row][col] != FLAGGED) {
					grid[row][col] = MINE;
				}
				if (!has_mine[row][col] && grid[row][col] == FLAGGED) {
					grid[row][col] = SAFE;
				}
			}
		}
	} else {
		for (int row = 0; row < game_stats[0]; row += 1) {
			for (int col = 0; col < game_stats[1]; col += 1) {
				if (has_mine[row][col] && grid[row][col] != FLAGGED) {
					flag(row, col);
				}
			}
		}
	}
	display_grid();
}

bool reveal_cell() {
	char col_letter;
	int row, col;
	cout << "Please enter cell coordinates: ";
	cin >> col_letter;
	cin >> row;
	col = int(col_letter - 'A');
	row = row - 1;

	if (row < 0 || row >= game_stats[0] || col < 0 || col >= game_stats[1]) {
		cout << "Given coordinates are out of bounds!" << '\n';
		return false;
	}
	if (grid[row][col] >= 0) {
		cout << "Selected cell is already revealed!" << '\n';
		return false;
	}
	if (grid[row][col] == FLAGGED) {
		cout << "Selected cell is currently flagged!" << '\n';
		return false;
	}

	reveal(row, col);
	return true;
}

bool toggle_flag() {
	char col_letter;
	int row, col;
	cout << "Please enter cell coordinates: ";
	cin >> col_letter;
	cin >> row;
	col = int(col_letter - 'A');
	row = row - 1;

	if (row < 0 || row >= game_stats[0] || col < 0 || col >= game_stats[1]) {
		cout << "Given coordinates are out of bounds!" << '\n';
		return false;
	}

	if (grid[row][col] >= 0) {
		cout << "Selected cell is already revealed!" << '\n';
		return false;
	}

	flag(row, col);
	if (game_stats[2] < 0) {
		cout << "WARNING: Too many flags on the grid!" << '\n';
	}
	return true;
}

bool solve_locally() {
	if (game_stats[4] > 0) {
		cout << "Incorrect flagging detected! Local Solver unavailable!"
			 << '\n';
		return false;
	}

	int to_reveal[MAX_CELLS][2];
	int to_flag[MAX_CELLS][2];
	int num_return[2];
	bool repeat = true;
	while (repeat) {
		repeat = local_solver(to_reveal, to_flag, num_return);

		for (int i = 0; i < num_return[0]; i += 1) {
			reveal(to_reveal[i][0], to_reveal[i][1]);
		}
		for (int j = 0; j < num_return[1]; j += 1) {
			flag(to_flag[j][0], to_flag[j][1]);
		}
	}
	return true;
}

bool solve_globally() {
	if (game_stats[4] > 0) {
		cout << "Incorrect flagging detected! Global Solver unavailable!"
			 << '\n';
		return false;
	}

	int to_reveal[MAX_CELLS][2];
	int to_flag[MAX_CELLS][2];
	int num_return[2];
	bool repeat_global = true;
	while (repeat_global) {
		bool repeat_local = true;
		while (repeat_local) {
			repeat_local = local_solver(to_reveal, to_flag, num_return);

			for (int i = 0; i < num_return[0]; i++) {
				reveal(to_reveal[i][0], to_reveal[i][1]);
			}
			for (int j = 0; j < num_return[1]; j++) {
				flag(to_flag[j][0], to_flag[j][1]);
			}
		}

		repeat_global = global_solver(to_reveal, to_flag, num_return);
		for (int i = 0; i < num_return[0]; i += 1) {
			reveal(to_reveal[i][0], to_reveal[i][1]);
		}
		for (int j = 0; j < num_return[1]; j += 1) {
			flag(to_flag[j][0], to_flag[j][1]);
		}

		break;

		display_grid();
	}
	return true;
}

int main() {
	cout << "Welcome to COMP 2012H PA 1!" << '\n';
	cout << "MINESWEEPER!" << '\n';
	cout << '\n';

	generate_grid();

	// Game Loop.
	while (!mine_exploded && game_stats[3] > 0) {
		display_grid();

		// Player Action Loop.
		bool valid_input = false;
		while (!valid_input) {
			cout << "Choose one of the following actions:" << '\n';
			cout << "0: Reveal cell" << '\n';
			cout << "1: Toggle flag" << '\n';
			cout << "2: Solve locally" << '\n';
			cout << "3: Solve globally" << '\n';
			int player_action;
			cin >> player_action;

			switch (player_action) {
				case 0:
					valid_input = reveal_cell();
					break;
				case 1:
					valid_input = toggle_flag();
					break;
				case 2:
					valid_input = solve_locally();
					break;
				case 3:
					valid_input = solve_globally();
					break;
				default:
					valid_input = false;
					cout << "Invalid input. Please enter again.\n" << '\n';
					break;
			}
		}
	}
	display_answer();

	if (mine_exploded) {
		cout << "A MINE EXPLODED! GAME OVER! YOU LOSE!" << '\n';
	} else {
		cout << "CONGRATULATIONS! YOU WIN!" << '\n';
	}

	// Prevent Immediate Console Exit.
	cout << '\n';
	cout << "Press [ENTER] to exit..." << '\n';
	cin.ignore();  // Clear the newline character from the input buffer.
	cin.get();	// Wait for a single instance of cin input, but not do anything
				// with it.

	return 0;
}
