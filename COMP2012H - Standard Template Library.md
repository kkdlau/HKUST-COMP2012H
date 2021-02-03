# COMP2012H - Standard Template Library

Note: https://course.cse.ust.hk/comp2012h/notes/14-stl-full.pdf

Table of Content
[TOC]

## Class Template

* **Within** the template, the **typename** may be **omitted** (P.6)
  * And any member function implemented out of class, `<T>` must be inserted
  ```cpp
  template<typename T>
  class Test {
    Test(); // <T> is omitted
  }
  template<typename T>
  Test<T>::Test() {} // <T> cannot be omitted
  ```
  
  
## `Vector<T>`

* **`resize()`**: drop elements if `new_size` < `old_size`, otherwise insert `0`

* **`end()`**: return address after the last element


## Iterator

* behaves like `pointer`
* **`const_iterator`**: pointer to const(i.e. iterator itself can be changed)
* Split into **`begin`** and **`end`** parameter can allow user to specific the range(P.21)


### `find()`

* Don't design as member function: because normal pointer can work with same function

## Function Pointer

* Pointer of no argument function: `<T> (*funcs)(void)` <= you must put `void`
* Array of Function Pointer: `void (*f[])(void)`


## Function Object

* Using object to simulate function call

```cpp
class TEST {
  public: operator() (int k) {/**/}
}

TEST t;
t(100);
```
  `t` looks like a **function call**, but in fact `t` is an object.
  So we can it as an function object.
  
  
  
## `for_each`