# COMP2012H - Generic Programming

Note: https://course.cse.ust.hk/comp2012h/notes/13-generic-programming-full.pdf

Table of Content
[TOC]


## Function / Class Template
```cpp
template <typename T> // typename = class
inline const T& max(const T& a, const T& b) {
  return a > b? a: b;
}
```
* Function template will create different functions based on `type`
  * Template itself is not a function

* cpp predicts `type` if not `<T>` specified
  * This feature is **not available to class template**

* The implementation of template cannot be splitted into `.h` and `.cpp`
  * because cpp doesn't full class details
  * It's recommended to put in `.h`

* If the template doesn't be used in anywhere, cpp **doesn't check anything about the template**! (Including syntax error)
### Template Specialization

```cpp
template <>
const char* const& max(const char* const& a, const char* const& b) {
  return str(a, b) < 0? b: a;
}
```

* `template <>` means special version of `max`

:::info
#### Ternary operator

```cpp
(a < b)? a: b
```
if `a` and `b` are different types, then the `a: b` both are stored in `temp`.

```cpp
const int＆ max(const int& a, const double& b) {
  return a > b? a : b; 
}
```
* Since `a` and `b` are different types -> `a` convert to `double` (i.e. a `temp` is created)
* Assume `b` is returned
  * create `temp` for `b` (**ternary operator creates `temp` if we compare with different type**)
  * return type mismatchs -> an `int` `temp` is created

**Therefore, a `temp` object is returned by reference.**

> further reading: https://www.geeksforgeeks.org/cc-ternary-operator-some-interesting-observations/
:::


### Nontype Parameters for Templates

* Only accepts types that can convert to `int`
  * e.g. `short`, `long`
  * Don't accept `float`, `double`, etc

* The parameters are constants

### Reference for Array with Templates
```cpp
int (&arr)[5];
```
It means size-5 `int` array reference.

```cpp
template <typename T, int N>
int f(T (&x)[N]) {
  return N;
}
```

cpp predicts `T`, and `N` automatically.


## Operator overloading

* Can define as global function (**You can't define both
* **Arity**: The number of arguments is fixed
* **Associativity**, and **Precedence** are fixed
* `operator=`, `operator[]` and `operator()` must be member function 
* **mutator keyword**: `const` is considered as function signature
  * so you can have two version of `[]`:
  ```cpp
  operator[](int i) const; // called const object
  operator[](int i);
  ```
  
  
## `friend` keyword
```cpp
class A {
  private:
    int a;
    friend void test();
    friend class B;
}

class B {/***/}

void test() {/***/}
```

* `class B` and `test()` are considered as within the class scope of `A`
  * they can access `private` members

* `friend` can be put on anywhere, including `public` and `protected`
  * the effect is the same

* **`friend function`** defined in class is global function (P.59)
  ```cpp
  class A {
    friend ostream& operator<<(ostream& os, const A& a) {
    /*
     * This function is global
     * even the definition is writien in class
     */
     
    }
  }
  ```
  
* `friend function / class` can be not exit, and no error about it 