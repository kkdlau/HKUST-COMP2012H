# COMP2012H - Static, Stack & Queue

Note: https://course.cse.ust.hk/comp2012h/notes/15-static-data-function-full.pdf

https://course.cse.ust.hk/comp2012h/notes/18-stack-queue-full.pdf

Table of Content
[TOC]

## Static Data Member
:::info
if a class is fully static => still hold memory?

if a class have static member that not initialized, can create instance?
:::

* The access of static data member is protected by `private`, `protect` and `public` keywords

* can be accessed by `object instance`, and `class`
  ```cpp
  class A {
    public:
      int a;
      static int b;
  }
  ```
  ```cpp
  A::b; // valid
  
  A obj1; obj1.b; // valid, same b as A::b
  A obj2; obj2.b; // valid, same b as A::b
  ```
  
* Static data member **isn't created** if the initialization isn't completed in class
  * `A::b` is the only way to initialize

## Static Memeber Function

* cannot overlord
* cannot be virtual or const
* no `this` pointer

## Private Constructor

* The constructor can be called by `static member function` (P.18)