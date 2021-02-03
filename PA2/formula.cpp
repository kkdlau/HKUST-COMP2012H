#include <iostream>
#include <cstring>

#include "formula.h"

using namespace std;

// Pretty-printer for propositional formulae
// N.B. It has no error-checking so invalid formulae leads to UB
void print_formula(const Formula *formula)
{
    // Don't crash on nullptr (this is the only error-checking this function performs)
    if (formula == nullptr)
        return;
    // Atoms and constants
    if (formula->type != Formula::CONNECTIVE)
    {
        cout << formula->name;
        return;
    }
    // Conjunction
    if (formula->name[0] == '/')
    {
        if (formula->left->name[0] == '/' || formula->left->name[0] == '\\' || (formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
            cout << "(";
        print_formula(formula->left);
        if (formula->left->name[0] == '/' || formula->left->name[0] == '\\' || (formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
            cout << ")";
        cout << " ";
        cout << formula->name;
        cout << " ";
        if (formula->right->name[0] == '\\' || (formula->right->name[0] == '-' && formula->right->right->name[0] != 'F'))
            cout << "(";
        print_formula(formula->right);
        if (formula->right->name[0] == '\\' || (formula->right->name[0] == '-' && formula->right->right->name[0] != 'F'))
            cout << ")";
        return;
    }
    // Disjunction
    if (formula->name[0] == '\\')
    {
        if (formula->left->name[0] == '\\' || (formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
            cout << "(";
        print_formula(formula->left);
        if (formula->left->name[0] == '\\' || (formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
            cout << ")";
        cout << " ";
        cout << formula->name;
        cout << " ";
        if ((formula->right->name[0] == '-' && formula->right->right->name[0] != 'F'))
            cout << "(";
        print_formula(formula->right);
        if ((formula->right->name[0] == '-' && formula->right->right->name[0] != 'F'))
            cout << ")";
        return;
    }
    // Implication (Negation)
    if (formula->right->name[0] == 'F')
    {
        cout << "~";
        if (formula->left->type == Formula::CONNECTIVE && (formula->left->name[0] != '-' || formula->left->right->name[0] != 'F'))
            cout << "(";
        print_formula(formula->left);
        if (formula->left->type == Formula::CONNECTIVE && (formula->left->name[0] != '-' || formula->left->right->name[0] != 'F'))
            cout << ")";
        return;
    }
    // Implication (general case)
    if ((formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
        cout << "(";
    print_formula(formula->left);
    if ((formula->left->name[0] == '-' && formula->left->right->name[0] != 'F'))
        cout << ")";
    cout << " ";
    cout << formula->name;
    cout << " ";
    print_formula(formula->right);
}

// Parser for propositional formulae
// N.B. It has no error-checking so invalid formulae leads to UB
Formula *parse_formula(char *str)
{
    // Don't crash on nullptr (this is the only error-checking this function performs)
    if (str == nullptr)
        return nullptr;
    // Implication
    char *p = str;
    while (*p)
    {
        if (*p == '-')
        {
            *p = '\0';
            Formula *left = parse_formula(str);
            Formula *right = parse_formula(p + 2);
            Formula *result = new Formula;
            result->type = Formula::CONNECTIVE;
            strcpy(result->name, "->");
            result->left = left;
            result->right = right;
            return result;
        }
        if (*p == '(')
        {
            int unmatched = 1;
            while (unmatched)
            {
                ++p;
                if (*p == '(')
                    ++unmatched;
                else if (*p == ')')
                    --unmatched;
            }
        }
        ++p;
    }
    // Disjunction
    p = str;
    while (*p)
    {
        if (*p == '\\' && *(p + 1) == '/')
        {
            *p = '\0';
            Formula *left = parse_formula(str);
            Formula *right = parse_formula(p + 2);
            Formula *result = new Formula;
            result->type = Formula::CONNECTIVE;
            strcpy(result->name, "\\/");
            result->left = left;
            result->right = right;
            return result;
        }
        if (*p == '(')
        {
            int unmatched = 1;
            while (unmatched)
            {
                ++p;
                if (*p == '(')
                    ++unmatched;
                else if (*p == ')')
                    --unmatched;
            }
        }
        ++p;
    }
    // Conjunction
    p = str;
    while (*p)
    {
        if (*p == '/' && *(p + 1) == '\\')
        {
            *p = '\0';
            Formula *left = parse_formula(str);
            Formula *right = parse_formula(p + 2);
            Formula *result = new Formula;
            result->type = Formula::CONNECTIVE;
            strcpy(result->name, "/\\");
            result->left = left;
            result->right = right;
            return result;
        }
        if (*p == '(')
        {
            int unmatched = 1;
            while (unmatched)
            {
                ++p;
                if (*p == '(')
                    ++unmatched;
                else if (*p == ')')
                    --unmatched;
            }
        }
        ++p;
    }
    // Negation / nested formula / constant / atom
    p = str;
    while (*p == ' ')
        ++p;
    if (*p == '~')
    {
        Formula *left = parse_formula(p + 1);
        Formula *right = formula_F();
        Formula *result = new Formula;
        result->type = Formula::CONNECTIVE;
        strcpy(result->name, "->");
        result->left = left;
        result->right = right;
        return result;
    }
    if (*p == '(')
    {
        char *q = p;
        int unmatched = 1;
        while (unmatched)
        {
            ++q;
            if (*q == '(')
                ++unmatched;
            else if (*q == ')')
                --unmatched;
        }
        *q = '\0';
        Formula *result = parse_formula(p + 1);
        return result;
    }
    if (*p == 'T')
    {
        Formula *result = new Formula;
        result->type = Formula::CONSTANT;
        strcpy(result->name, "T");
        result->left = nullptr;
        result->right = nullptr;
        return result;
    }
    if (*p == 'F')
        return formula_F();
    Formula *result = new Formula;
    result->type = Formula::ATOM;
    result->name[0] = *p;
    result->name[1] = '\0';
    result->left = nullptr;
    result->right = nullptr;
    return result;
}

// Identifier validator
// An identifier is considered valid in My Mini Proof Assistant (TM) if it is a
// C-style identifier, subject to the additional restriction that the first
// character cannot be uppercase
bool is_valid_id(const char *id)
{
    // nullptr is invalid ID
    if (id == nullptr)
        return false;
    // Empty ID is invalid
    if (*id == '\0')
        return false;
    // First character must be lowercase or underscore
    const char *ALLOWED_CHARS = "abcdefghijklmnopqrstuvwxyz_";
    bool valid = false;
    while (*ALLOWED_CHARS)
    {
        if (*id == *ALLOWED_CHARS)
        {
            valid = true;
            break;
        }
        ++ALLOWED_CHARS;
    }
    if (!valid)
        return false;
    // Other characters must be alphanumeric or underscore
    ++id;
    ALLOWED_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
    while (*id)
    {
        const char *allowed_chars = ALLOWED_CHARS;
        valid = false;
        while (*allowed_chars)
        {
            if (*id == *allowed_chars)
            {
                valid = true;
                break;
            }
            ++allowed_chars;
        }
        if (!valid)
            return false;
        ++id;
    }
    return true;
}

// Deallocatof for propositional formulae
void delete_formula(Formula *formula)
{
    if (formula != nullptr)
    {
        delete_formula(formula->left);
        delete_formula(formula->right);
        delete formula;
    }
}

// Create a deep copy of a propositional formula
Formula *formula_deep_copy(const Formula *formula)
{
    if (formula == nullptr)
        return nullptr;
    Formula *result = new Formula;
    result->type = formula->type;
    strcpy(result->name, formula->name);
    result->left = formula_deep_copy(formula->left);
    result->right = formula_deep_copy(formula->right);
    return result;
}

// Check whether two ASTs represent the same propositional formula
bool formulae_equal(const Formula *p, const Formula *q)
{
    if (p == nullptr && q == nullptr)
        return true;
    if (p == nullptr || q == nullptr)
        return false;
    return p->type == q->type &&
           strcmp(p->name, q->name) == 0 &&
           formulae_equal(p->left, q->left) &&
           formulae_equal(p->right, q->right);
}

// formula == T?
bool formula_is_T(const Formula *formula)
{
    if (formula == nullptr)
        return false;
    return formula->name[0] == 'T';
}

// Returns the formula F (dynamically allocated)
Formula *formula_F()
{
    Formula *result = new Formula;
    result->type = Formula::CONSTANT;
    strcpy(result->name, "F");
    result->left = nullptr;
    result->right = nullptr;
    return result;
}

// Checks whether the propositional formula is a disjunction
bool formula_is_disj(const Formula *formula)
{
    if (formula == nullptr)
        return false;
    return formula->name[0] == '\\';
}

// The left side of a disjunction
Formula *&formula_disj_left(Formula *formula)
{
    if (!formula_is_disj(formula))
    {
        cerr << "Fatal error: cannot get the left side of a disjunction when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->left;
}

// The right side of a disjunction
Formula *&formula_disj_right(Formula *formula)
{
    if (!formula_is_disj(formula))
    {
        cerr << "Fatal error: cannot get the right side of a disjunction when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->right;
}

// Checks whether the propositional formula is an implication
bool formula_is_impl(const Formula *formula)
{
    if (formula == nullptr)
        return false;
    return formula->name[0] == '-';
}

// The antecedent in an implication
Formula *&formula_impl_antecedent(Formula *formula)
{
    if (!formula_is_impl(formula))
    {
        cerr << "Fatal error: cannot get the antecedent of an implication when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->left;
}

// The consequent in an implication
Formula *&formula_impl_consequent(Formula *formula)
{
    if (!formula_is_impl(formula))
    {
        cerr << "Fatal error: cannot get the consequent of an implication when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->right;
}

// Checks whether the propositional formula is a conjunction
bool formula_is_conj(const Formula *formula)
{
    if (formula == nullptr)
        return false;
    return formula->name[0] == '/';
}

// The left side of a conjunction
Formula *&formula_conj_left(Formula *formula)
{
    if (!formula_is_conj(formula))
    {
        cerr << "Fatal error: cannot get the left side of a conjunction when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->left;
}

// The right side of a conjunction
Formula *&formula_conj_right(Formula *formula)
{
    if (!formula_is_conj(formula))
    {
        cerr << "Fatal error: cannot get the right side of a conjunction when there is none!" << endl;
        exit(EXIT_FAILURE);
    }
    return formula->right;
}

// formula == F?
bool formula_is_F(const Formula *formula)
{
    if (formula == nullptr)
        return false;
    return formula->name[0] == 'F';
}

// Given a proposition P, returns its negation ~P (dynamically allocated)
Formula *formula_neg_of(Formula *formula)
{
    Formula *result = new Formula;
    result->type = Formula::CONNECTIVE;
    strcpy(result->name, "->");
    result->left = formula;
    result->right = formula_F();
    return result;
}