#include "mmpa.h"

#include <cstring>
#include <iostream>

#include "formula.h"
// #define DEBUG

#ifdef DEBUG
#define debug_print(msg) cout << msg << endl;
#else
#define debug_print(msg)
#endif

using namespace std;

/**
 * return true if two given char array is the same.
 *
 * @param s1 char array 1
 * @param s2 char array 2
 *
 * @return true if two given char array is the same.
 */
inline bool string_match(const char *s1, const char *s2) {
	return strcmp(s1, s2) == 0;
}

inline char *const str_dynamic_clone(const char *src) {
	char *dynamic_str = new char[strlen(src) + 1];
	strcpy(dynamic_str, src);
	return dynamic_str;
}

Context *context_init(Formula *conclusion, Theorem *premises = nullptr,
					  Context *prev = nullptr, Context *next = nullptr) {
	Context *ptr = new Context;
	ptr->conclusion = conclusion;
	ptr->premises = premises;
	ptr->prev = prev != nullptr ? prev : ptr;
	ptr->next = next != nullptr ? next : ptr;
	return ptr;
}

void insert_context(Context *&node, Context *new_node) {
	if (!node) {
		node = new_node;
		return;
	}
	Context *next = node->next;
	node->next = new_node;
	new_node->next = next;
	next->prev = new_node;
	new_node->prev = node;
}

inline void delete_single_context(Context *cntxt) {
	delete_formula(cntxt->conclusion);
	delete_thms(cntxt->premises);
	Context *next = cntxt->next;
	Context *prev = cntxt->prev;
	next->prev = prev;
	prev->next = next;
	debug_print("deleted a context");
	delete cntxt;
}

Theorem *theorem_init(const char *const name = nullptr, Formula *stmt = nullptr,
					  Theorem *next = nullptr) {
	Theorem *thm = new Theorem;
	if (name) thm->name = str_dynamic_clone(name);
	thm->stmt = stmt;
	thm->next = next;
	return thm;
}

Theorem *theorem_clone(Theorem *thm) {
	return theorem_init(thm->name, formula_deep_copy(thm->stmt), thm->next);
}

Theorem *theorem_clone_list(Theorem *thm) {
	Theorem *parent = theorem_clone(thm);
	Theorem *head = parent;

	for (thm = thm->next; thm != nullptr; thm = thm->next) {
		Theorem *node = theorem_clone(thm);
		parent->next = node;
		parent = node;
	}

	return head;
}

Theorem *last_theorem(Theorem *node) {
	if (node == nullptr) return nullptr;
	if (node->next)
		last_theorem(node->next);
	else
		return node;

	return nullptr;
}

template <typename F>
Theorem *find_theorem(Theorem *theorem, F is_target) {
	if (theorem == nullptr) {
		return nullptr;
	} else {
		if (is_target(theorem->stmt)) {
			debug_print("search algorithm found");
			return theorem;
		};
		find_theorem(theorem->next, is_target);
	}
}

/*
 * mmpa.cpp
 * Implementation file for My Mini Proof Assistant (TM)
 * You should submit ONLY this file
 */

void print_thms(const Theorem *thms) {
	while (thms != nullptr) {
		cout << thms->name;
		cout << " : ";
		print_formula(thms->stmt);
		cout << endl;
		thms = thms->next;
	}
}

void display_focus(const ProofState &proof_state) {
	if (proof_state.focus == nullptr) return;
	Theorem *premises = proof_state.focus->premises;
	print_thms(premises);
	cout << "__________________________________________________________________"
			"______________"
		 << endl;
	print_formula(proof_state.focus->conclusion);
	cout << endl;
}

// Task 1: Preliminaries
void delete_thms(Theorem *thms) {
	if (thms != nullptr) {
		delete_thms(thms->next);
		delete[] thms->name;
		delete_formula(thms->stmt);
		thms->name = nullptr;
		delete thms;
	}
}

const Theorem *id_exists(const char *id, const Theorem *thms) {
	const Theorem *current_theorm = thms;
	while (current_theorm != nullptr) {
		if (string_match(current_theorm->name, id)) return current_theorm;
		current_theorm = current_theorm->next;
	}

	return nullptr;
}

void init_proof(ProofState &proof_state, Formula *goal) {
	proof_state.focus = context_init(goal);
}

bool has_subgoals(const ProofState &proof_state) {
	return proof_state.focus != nullptr;
}

void add_thm_to_database(Theorem *&thms, const char *name, Formula *stmt) {
	Theorem *to_add_thm = theorem_init(name, stmt, thms);
	thms = to_add_thm;	// update the pointer of theorom to the latest
}

int num_subgoals(const ProofState &proof_state) {
	if (!proof_state.focus) return 0;
	int count = 1;
	Context *stop_node = proof_state.focus;
	for (Context *sub_goal = proof_state.focus->next; sub_goal != stop_node;
		 sub_goal = sub_goal->next)
		count++;
	return count;
}

void delete_context(Context *cntxt, Context *stop_node) {
	if (cntxt->prev == cntxt && cntxt->next == cntxt) {
		// only single context
		debug_print("case 1");
		delete_single_context(cntxt);
		return;
	}

	debug_print("case 2");
	// otherwise, use recursion
	if (cntxt != stop_node) {
		delete_context(cntxt->next, stop_node);
		delete_single_context(cntxt);
	} else {
		delete_single_context(cntxt);
	}
}

void clear_proof_state(ProofState &proof_state) {
	delete_context(proof_state.focus->prev, proof_state.focus);
	proof_state.focus = nullptr;
}

inline void update_conclusion(ProofState &proof_state, Formula *conclusion) {
	delete_formula(proof_state.focus->conclusion);
	proof_state.focus->conclusion = conclusion;
}

void finish_subgoal(ProofState &proof_state) {
	Context *next_subgoal = proof_state.focus->next;
	delete_single_context(proof_state.focus);
	proof_state.focus =
		next_subgoal != proof_state.focus ? next_subgoal : nullptr;
}

// Task 2: Tactics for constructive logic, Part I
void tactic_trivial(ProofState &proof_state) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	if (formula_is_T(focus->conclusion))
		finish_subgoal(proof_state);
	else
		cout << "Tactic 'trivial' failed: conclusion is not 'T'" << endl;
}

void tactic_exfalso(ProofState &proof_state) {
	if (proof_state.focus) {
		update_conclusion(proof_state, formula_F());
	}
}

void tactic_left(ProofState &proof_state) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	Formula *&conclusion = focus->conclusion;
	if (formula_is_disj(conclusion)) {
		Formula *left_part = formula_deep_copy(formula_disj_left(conclusion));
		update_conclusion(proof_state, left_part);
	} else {
		cout << "Tactic 'left' failed: conclusion is not a disjunction" << endl;
	}
}

void tactic_right(ProofState &proof_state) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	Formula *&conclusion = focus->conclusion;
	if (formula_is_disj(conclusion)) {
		Formula *right_part = formula_deep_copy(formula_disj_right(conclusion));
		update_conclusion(proof_state, right_part);
	} else {
		cout << "Tactic 'right' failed: conclusion is not a disjunction"
			 << endl;
	}
}

void tactic_intro(ProofState &proof_state, const char *hyp) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	Formula *&conclusion = focus->conclusion;
	if (formula_is_impl(conclusion)) {
		if (id_exists(hyp, focus->premises)) {
			cout << "Tactic 'intro' failed: hypothesis name is already taken"
				 << endl;
		} else {
			Formula *antecedent =
				formula_deep_copy(formula_impl_antecedent(conclusion));
			Formula *consequent =
				formula_deep_copy(formula_impl_consequent(conclusion));
			add_thm_to_database(focus->premises, hyp, antecedent);
			update_conclusion(proof_state, consequent);
		}
	} else {
		cout << "Tactic 'intro' failed: conclusion is not an implication"
			 << endl;
	}
}

void tactic_split(ProofState &proof_state) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	Formula *&conclusion = focus->conclusion;
	if (formula_is_conj(conclusion)) {
		Context *new_sub_goal =
			context_init(formula_deep_copy(formula_conj_right(conclusion)),
						 theorem_clone_list(focus->premises));
		update_conclusion(proof_state,
						  formula_deep_copy(formula_conj_left(conclusion)));

		insert_context(proof_state.focus, new_sub_goal);
	} else {
		cout << "Tactic 'split' failed: conclusion is not a conjunction"
			 << endl;
	}
}

// Task 3: Tactics for constructive logic, Part II

void tactic_contradiction(ProofState &proof_state,
						  const Theorem *proven_results) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	if (find_theorem(focus->premises, formula_is_F))
		finish_subgoal(proof_state);
	else {
		cout << "Tactic 'contradiction' failed: no contradiction found in "
				"local or global context"
			 << endl;
	}
}

void destruct_conj_theorm(ProofState &proof_state, Formula *f, const char *hyp1,
						  const char *hyp2) {
	add_thm_to_database(proof_state.focus->premises, hyp2,
						formula_deep_copy(formula_conj_right(f)));
	add_thm_to_database(proof_state.focus->premises, hyp1,
						formula_deep_copy(formula_conj_left(f)));
}

void destruct_disj_theorm(ProofState &proof_state, Formula *f, const char *hyp1,
						  const char *hyp2) {
	Context *new_sub_goal =
		context_init(formula_deep_copy(proof_state.focus->conclusion),
					 theorem_clone_list(proof_state.focus->premises));
	add_thm_to_database(proof_state.focus->premises, hyp1,
						formula_deep_copy(formula_disj_left(f)));
	add_thm_to_database(new_sub_goal->premises, hyp2,
						formula_deep_copy(formula_disj_right(f)));

	insert_context(proof_state.focus, new_sub_goal);
}

void tactic_destruct(ProofState &proof_state, const Theorem *proven_results,
					 const char *hyp, const char *hyp1, const char *hyp2) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	const Theorem *target = id_exists(hyp, focus->premises);
	if (!target) target = id_exists(hyp, proven_results);
	if (!target) {
		cout << "Tactic 'destruct' failed: given hypothesis name not found in "
				"local or global context"
			 << endl;
		return;
	}
	if (id_exists(hyp1, focus->premises) || id_exists(hyp2, focus->premises)) {
		cout << "Tactic 'destruct' failed: hypothesis name is already taken"
			 << endl;
		return;
	}

	if (formula_is_conj(target->stmt)) {
		if (string_match(hyp1, hyp2)) {
			cout << "Tactic 'destruct' failed: the two hypothesis names must "
					"be unique"
				 << endl;
			return;
		}
		destruct_conj_theorm(proof_state, target->stmt, hyp1, hyp2);
	} else if (formula_is_disj(target->stmt)) {
		destruct_disj_theorm(proof_state, target->stmt, hyp1, hyp2);
	} else {
		cout << "Tactic 'destruct' failed: the given hypothesis is not a "
				"conjunction or disjunction"
			 << endl;
	}
}

Formula *right_most_atom(Formula *f) {
	if (!formula_is_impl(f)) return nullptr;
	if (f->type != Formula::CONNECTIVE)
		return f;
	else
		return right_most_atom(formula_impl_consequent(f));
}

bool is_valid_apply_prime(Formula *f, const Formula *conclusion) {
	if (formula_is_impl(f)) {
		return formulae_equal(right_most_atom(f), conclusion);
	} else
		return false;
}

void tactic_apply(ProofState &proof_state, const Theorem *proven_results,
				  const char *hyp) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	const Theorem *exit_hyp = id_exists(hyp, focus->premises);
	if (!exit_hyp) exit_hyp = id_exists(hyp, proven_results);
	if (!exit_hyp) {
		cout
			<< "Tactic 'apply' failed: hypothesis not found in local or global "
			   "context"
			<< endl;
		return;
	}

	if (formulae_equal(exit_hyp->stmt, focus->conclusion)) {
		finish_subgoal(proof_state);
		return;
	} else if (is_valid_apply_prime(exit_hyp->stmt,
									proof_state.focus->conclusion)) {
		Context *new_sub_goal;
		Context *last_sub_goal = focus;
		Formula *remaining = exit_hyp->stmt;
		Context *source_focus = focus;
		do {
			new_sub_goal = context_init(
				formula_deep_copy(formula_impl_antecedent(remaining)),
				theorem_clone_list(source_focus->premises));
			insert_context(last_sub_goal, new_sub_goal);
			last_sub_goal = new_sub_goal;
			remaining = formula_impl_consequent(remaining);
		} while (!formulae_equal(remaining, source_focus->conclusion));

		Context *new_head = source_focus->next;
		delete_single_context(source_focus);
		focus = new_head;
	} else if (formula_is_impl(exit_hyp->stmt)) {
		if (formulae_equal(formula_impl_consequent(exit_hyp->stmt),
						   focus->conclusion))
			update_conclusion(
				proof_state,
				formula_deep_copy(formula_impl_antecedent(exit_hyp->stmt)));
		else
			cout << "Tactic 'apply' failed: hypothesis is not applicable to "
					"conclusion"
				 << endl;
	} else {
		cout << "Tactic 'apply' failed: hypothesis is not applicable to "
				"conclusion"
			 << endl;
	}
}

// Task 4: Tactics for classical logic

void tactic_excluded_middle(ProofState &proof_state, const char *hyp,
							Formula *ex_mid_P) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	if (id_exists(hyp, focus->premises)) {
		cout << "Tactic 'excluded_middle' failed: hypothesis name is already "
				"taken"
			 << endl;
		return;
	}

	Context *new_sub_goal = context_init(formula_deep_copy(focus->conclusion),
										 theorem_clone_list(focus->premises));
	add_thm_to_database(new_sub_goal->premises, hyp,
						formula_neg_of(formula_deep_copy(ex_mid_P)));
	add_thm_to_database(focus->premises, hyp, formula_deep_copy(ex_mid_P));

	insert_context(focus, new_sub_goal);
}

void tactic_by_contradiction(ProofState &proof_state, const char *hyp) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	if (id_exists(hyp, focus->premises)) {
		cout << "Tactic 'by_contradiction' failed: hypothesis name is already "
				"taken"
			 << endl;
		return;
	}
	debug_print("ok1");
	add_thm_to_database(focus->premises, hyp,
						formula_neg_of(formula_deep_copy(focus->conclusion)));
	debug_print("ok2");

	update_conclusion(proof_state, formula_F());
	debug_print("ok3");
}

// Task 5: Tactics for proof management

void tactic_rotate(ProofState &proof_state, int n) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;
	for (int i = 0; i < n; i++) focus = focus->next;
}

void tactic_clear(ProofState &proof_state, const char *hyp) {
	Context *&focus = proof_state.focus;
	if (focus == nullptr) return;

	Theorem *prev_thm = nullptr;
	Theorem *thm = proof_state.focus->premises;
	while (thm != nullptr && !string_match(thm->name, hyp)) {
		prev_thm = thm;
		thm = thm->next;
	}

	if (thm && prev_thm == nullptr) {
		// head node
		focus->premises = thm->next;
		delete[] thm->name;
		delete_formula(thm->stmt);
		delete thm;
	} else if (thm) {
		prev_thm->next = thm->next;
		delete[] thm->name;
		delete_formula(thm->stmt);
		delete thm;
	} else {
		cout << "Tactic 'clear' failed: no hypothesis of the given name found"
			 << endl;
	}
}
