#include <iostream>
using namespace std;
#include "formula.h"
#include "mmpa.h"

/*
 * main.cpp
 * Driver program for My Mini Proof Assistant (TM)
 * Do NOT modify this file!
 */

int main(int argc, char **argv) {
	cout << "Welcome!" << endl;
	cout << "You are using My Mini Proof Assistant (TM), v0.1.0beta" << endl;
	Theorem *proven_results = nullptr;
	while (true) {
		cout << "Choose an option:" << endl;
		cout << "P. Prove a theorem" << endl;
		cout << "B. Browse existing theorems" << endl;
		cout << "Q. Quit" << endl;
		cout << "> ";
		char opt;
		cin >> opt;
		cin.ignore();
		char buf[1025], buf2[1025];
		bool valid_thm_name, thm_name_taken;
		Formula *stmt, *goal;
		ProofState proof_state;
		bool aborted;
		switch (toupper(opt)) {
			case 'P':
				cout << "Enter the name of the theorem to be proven:" << endl;
				cout << "> ";
				cin.getline(buf, sizeof(buf));
				valid_thm_name = is_valid_id(buf);
				thm_name_taken = id_exists(buf, proven_results);
				while (!valid_thm_name || thm_name_taken) {
					if (!valid_thm_name)
						cout << "Theorem name is invalid: name must be at "
								"least 1 character long, contain only "
								"alphanumeric characters + underscore and "
								"first character cannot be a digit or uppercase"
							 << endl;
					else
						cout << "There is already a theorem with the same "
								"name, please choose a different one."
							 << endl;
					cout << "Enter the name of the theorem to be proven:"
						 << endl;
					cout << "> ";
					cin.getline(buf, sizeof(buf));
					valid_thm_name = is_valid_id(buf);
					thm_name_taken = id_exists(buf, proven_results);
				}
				cout << "Enter the statement of the theorem to be proven:"
					 << endl;
				cout << "> ";
				cin.getline(buf2, sizeof(buf2));
				stmt = parse_formula(buf2);
				goal = formula_deep_copy(stmt);
				init_proof(proof_state, goal);
				aborted = false;
				while (has_subgoals(proof_state)) {
					cout << num_subgoals(proof_state)
						 << " subgoal(s) remaining." << endl;
					cout << "Current focused subgoal:" << endl;
					display_focus(proof_state);
					cout << "Enter a tactic:" << endl;
					cout << "> ";
					cin.getline(buf2, sizeof(buf2));
					char *tac = buf2;
					while (*tac == ' ') ++tac;
					switch (*tac) {
						case 't':
							// trivial
							tactic_trivial(proof_state);
							break;
						case 'c':
							if (tac[1] == 'o')
								// contradiction
								tactic_contradiction(proof_state,
													 proven_results);
							else {
								// clear
								int offset = 6;
								while (tac[offset] == ' ') ++offset;
								if (!is_valid_id(tac + offset))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else
									tactic_clear(proof_state, tac + offset);
							}
							break;
						case 'e':
							if (tac[2] == 'f')
								// exfalso
								tactic_exfalso(proof_state);
							else {
								// excluded_middle
								int offset = 16;
								while (tac[offset] == ' ') ++offset;
								int offset2 = offset;
								while (tac[offset2] != ' ' &&
									   tac[offset2] != ':')
									++offset2;
								tac[offset2] = '\0';
								if (!is_valid_id(tac + offset))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else {
									while (tac[offset2] != ':') ++offset2;
									Formula *ex_mid_hyp =
										parse_formula(tac + offset2 + 1);
									tactic_excluded_middle(
										proof_state, tac + offset, ex_mid_hyp);
									delete_formula(ex_mid_hyp);
								}
							}
							break;
						case 'i':
							// intro
							{
								int offset = 6;
								while (tac[offset] == ' ') ++offset;
								if (!is_valid_id(tac + offset))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else
									tactic_intro(proof_state, tac + offset);
							}
							break;
						case 'a':
							if (tac[1] == 'p') {
								// apply
								int offset = 6;
								while (tac[offset] == ' ') ++offset;
								if (!is_valid_id(tac + offset))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else
									tactic_apply(proof_state, proven_results,
												 tac + offset);
							} else {
								// abort
								clear_proof_state(proof_state);
								aborted = true;
							}
							break;
						case 'd':
							// destruct
							{
								int offset = 9;
								while (tac[offset] == ' ') ++offset;
								int offset2 = offset;
								while (tac[offset2] != ' ') ++offset2;
								tac[offset2] = '\0';
								++offset2;
								while (tac[offset2] != 'a') ++offset2;
								offset2 += 2;
								while (tac[offset2] == ' ') ++offset2;
								int offset3 = offset2;
								while (tac[offset3] != ' ') ++offset3;
								tac[offset3] = '\0';
								++offset3;
								while (tac[offset3] == ' ') ++offset3;
								if (!is_valid_id(tac + offset) ||
									!is_valid_id(tac + offset2) ||
									!is_valid_id(tac + offset3))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else
									tactic_destruct(proof_state, proven_results,
													tac + offset, tac + offset2,
													tac + offset3);
							}
							break;
						case 's':
							// split
							tactic_split(proof_state);
							break;
						case 'l':
							// left
							tactic_left(proof_state);
							break;
						case 'r':
							if (tac[1] == 'i')
								// right
								tactic_right(proof_state);
							else {
								// rotate
								int offset = 7;
								while (tac[offset] == ' ') ++offset;
								tactic_rotate(proof_state, atoi(tac + offset));
							}
							break;
						case 'b':
							// by_contradiction
							{
								int offset = 17;
								while (tac[offset] == ' ') ++offset;
								if (!is_valid_id(tac + offset))
									cout << "Tactic failed: invalid hypothesis "
											"name"
										 << endl;
								else
									tactic_by_contradiction(proof_state,
															tac + offset);
							}
							break;
						default:
							cout << "Error: unrecognized tactic" << endl;
					}
				}
				if (!aborted) {
					cout << "No more subgoals." << endl;
					add_thm_to_database(proven_results, buf, stmt);
					cout << "Theorem added to existing results" << endl;
				} else {
					cout << "Proof aborted" << endl;
					delete_formula(stmt);
				}
				break;
			case 'B':
				print_thms(proven_results);
				break;
			case 'Q':
				cout << "See you next time!" << endl;
				delete_thms(proven_results);
				return 0;
			default:
				cout << "Invalid option" << endl;
		}
	}
	return 0;
}
