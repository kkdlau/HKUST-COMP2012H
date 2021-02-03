# COMP2012H - Class and Object

Note: https://course.cse.ust.hk/comp2012h/notes/10-class-object-4page.pdf

Table of Content
[TOC]

## Chaining function

```cpp
class Test {
  public:
    Test(int n) {/***/}
    Test add(const Test& t) {return t;}
}

int main() {
  Test t(1);
  t.add(10).add(20);
}
```

Two temporary objects are created.

1. temp_1: `t.add(10)`
2. temp_2: `t.add(20)`

After the execution (Last In Fisrt Out):

1. `temp_2` is destroyed
2. Then `temp_1` is destroyed.


## Constant Object

* All data memebs are constant after initialization.

## const references as function argument

* you can assign value to const referene
  * if assigning value to const reference
  * cpp creates a `temporary object`
  * and share the object to the `const reference`

* Assigning value to reference is not valid