#ifndef _MMPA_H_
#define _MMPA_H_

#include <iostream>

#include "formula.h"

/*
 * mmpa.hpp
 * Header file for My Mini Proof Assistant (TM)
 * Do NOT modify this file!
 */

// Theorem database
// We store our list of previously proven results in a singly linked list
struct Theorem {
	// The name of our theorem
	char *name;
	// The statement of our theorem
	Formula *stmt;
	// Other theorems
	Theorem *next;
};

// Proof state
// A proof state consists of zero or more subgoals, where each subgoal is a
// context with a list of premises and a desired conclusion
// Note that a premise can be thought of as a "local theorem", i.e. something we
// know or assume to be true locally
struct Context {
	// List of local facts
	Theorem *premises;
	// Desired conclusion
	Formula *conclusion;
	// Previous/next subgoal
	Context *prev;
	Context *next;
};
struct ProofState {
	// Current focused subgoal
	Context *focus;
};

// Your tasks

// Task 1: Preliminaries
void init_thm(Theorem *, const char *, const Formula *,
			  Theorem *next = nullptr);
void print_thms(const Theorem *);
void delete_thms(Theorem *);
const Theorem *id_exists(const char *, const Theorem *);
void init_proof(ProofState &, Formula *);
bool has_subgoals(const ProofState &);
void add_thm_to_database(Theorem *&, const char *, Formula *);
int num_subgoals(const ProofState &);
void display_focus(const ProofState &);
void clear_proof_state(ProofState &);

// Task 2: Tactics for constructive logic, Part I
void tactic_trivial(ProofState &);
void tactic_exfalso(ProofState &);
void tactic_left(ProofState &);
void tactic_right(ProofState &);
void tactic_intro(ProofState &, const char *);
void tactic_split(ProofState &);

// Task 3: Tactics for constructive logic, Part II
void tactic_contradiction(ProofState &, const Theorem *);
void tactic_destruct(ProofState &, const Theorem *, const char *, const char *,
					 const char *);
void tactic_apply(ProofState &, const Theorem *, const char *);

// Task 4: Tactics for classical logic
void tactic_excluded_middle(ProofState &, const char *, Formula *);
void tactic_by_contradiction(ProofState &, const char *);
// void tactic_prem_intro(ProofState &, const Formula *, const char *);

// Task 5: Tactics for proof management
void tactic_rotate(ProofState &, int);
void tactic_clear(ProofState &, const char *);

#endif