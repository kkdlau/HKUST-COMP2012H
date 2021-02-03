#ifndef _FORMULA_H_
#define _FORMULA_H_

// Propositional formulae
// A propositional formula is defined recursively as follows (in pseudo-BNF):
//
//   formula ::= T | F | P | formula /\ formula | formula \/ formula | formula -> formula
//
// Here, T stands for truth/tautology/top, F stands for falsehood/contradiction/bottom,
// P stands for a propositional variable (sometimes called an "atom"), /\ stands for
// logical conjunction (e.g. P /\ Q is "P and Q"), \/ for logical disjunction (e.g.
// P \/ Q is "P or Q") and -> for logical implication (e.g. P -> Q is "P implies Q")
// In C++, we encode a propositional formula as a recursive tree structure, where
// the logical constants T/F and atoms are leaves (i.e. have no children), and the
// three logical operators /\, \/, -> are nodes with exactly 2 children
// We do not directly encode negation here since the negation ~P of some propositional
// formula P is defined as shorthand for P -> F and is treated only in the parsing /
// pretty-printing phase

struct Formula
{
    // The type of this formula
    // A propositional formula is either a constant (T/F), atom (P/Q/R/etc.) or connective
    // (/\ / \/ / ->)
    enum Type
    {
        CONSTANT,
        ATOM,
        CONNECTIVE
    };
    Type type;

    // Name/symbol of this part of the formula
    // This is at most 2 characters in length, excluding the trailing NUL byte
    char name[3];

    // Subformulae of this formula, if any (e.g. the subformulae of P -> Q are P and Q)
    // These are encoded as left and right subtrees of the corresponding tree in C++
    // For logical constants and atoms, both fields should be nullptr; otherwise, e.g. in
    // P -> Q, the left subtree is P and the right subtree is Q
    Formula *left;
    Formula *right;
};

// Utility functions (already implemented for you)

// Pretty-printer for propositional formulae
void print_formula(const Formula *);

// Parser for propositional formulae
Formula *parse_formula(char *);

// Identifier validator
bool is_valid_id(const char *);

// Deallocator for propositional formulae
void delete_formula(Formula *);

// Create a deep copy of a propositional formula
Formula *formula_deep_copy(const Formula *);

// Check whether two ASTs represent the same propositional formula
bool formulae_equal(const Formula *, const Formula *);

// formula == T?
bool formula_is_T(const Formula *);

// Returns the formula F (dynamically allocated)
Formula *formula_F();

// Checks whether the propositional formula is a disjunction
bool formula_is_disj(const Formula *);

// The left side of a disjunction
Formula *&formula_disj_left(Formula *);

// The right side of a disjunction
Formula *&formula_disj_right(Formula *);

// Checks whether the propositional formula is an implication
bool formula_is_impl(const Formula *);

// The antecedent in an implication
Formula *&formula_impl_antecedent(Formula *);

// The consequent in an implication
Formula *&formula_impl_consequent(Formula *);

// Checks whether the propositional formula is a conjunction
bool formula_is_conj(const Formula *);

// The left side of a conjunction
Formula *&formula_conj_left(Formula *);

// The right side of a conjunction
Formula *&formula_conj_right(Formula *);

// formula == F?
bool formula_is_F(const Formula *);

// Given a proposition P, returns its negation ~P (dynamically allocated)
Formula *formula_neg_of(Formula *);

#endif