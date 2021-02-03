# COMP2012H - Inheritance and Polymorphism

Note: https://course.cse.ust.hk/comp2012h/notes/12-inheritance-4page.pdf

Table of Content
[TOC]

## Inheritance

* **Derived / Sub Class**: class that inherit other classes

* **Based / Super Class**: class that to be inherited

* Syntax:
  ```html
  class <your_class>: public <to_inhert> {}
  ```
  
* Call Inheritance constructor by MIL:
  ```html
  <your_class>(/***/): <to_inhert>(<parameter>) {}
  ```
  
* `Data members` and `member functions` are grabbed, but `constructors` and `destructors` don't

## Substitution Principle

* benefit: allow functions to be resued by `derived calsses`

### Destructor

* `A* obj = new B`: A destructor is called when B is destructed (**not call B destructor!**)
* solve by virtual destructor

### Assigning Derived Class to Base Class

* The below statement is valid
  ```cpp
  /**
   * A: base class
   * B: derived class
   */
  A* r = new B;
  ```
  
### Function Resolution (P.18, 19)
Consider the following three function:
```cpp
void print(UPerson* p); // 1
void print(UPerson& p); // 2
void print(Student& p); // 3
```

* `function 3` is called
  ```cpp
  Student tom;
  print(tom);
  ```
  
* `function 2` is called
  ```cpp
  UPerson& tom2 = tom;
  print(Tom); // the type of tom is not considered
  ```
  
* `function 1` is called
  ```cpp
  UPerson* p = &tom;
  print(p);
  ```
  
## Constructor for Derived Class

* If base class constructor **doesn't be called in derived class constructor MIL**, cpp will **insert default base class constructor** on MIL
* Derived Class **always call base class constructor frist**


## Destructor for Derived Class

* No post-tasks for any type of destructor
* First call derived class destructor
* Then destroy data member
* Finally base class destructor


## Slicing

* **Slicing**: occur when copying derived class to base class (i.e. Object assignment)
  * **Won't occur** during `pointer assignment`, `referencing`
  * Only base class data members are copied


## Name Conflict

* **Name Conflict**: occurs when dervied class has the same data members in base class in temrs of naming
* Solution:
  * Access the base class member using `::` (class scope operator)


## Protected

* accessible to member functions, friends of the class, and derived classes

### Virtual Function

* Base Class functions are overrided by Derived Class functions
* Only make function virual if the function has more than one implementation

  ```cpp
  class A {
    public: virtual test() {}
  }

  class B: public A {
    public: test() {} // B can override test
  }

  class C: public B {
    public: test() {} // C can override test
  }
  ```
  `test()` in C overrides `test()` in A. So:
  ```cpp
  B* ptr = new C;
  ptr->test(); // runtime confirm which test to call
  ```
  `test()` in C is called (dynmaic binding).
  
  ```cpp
  B* ptr = new C;
  ptr->A::test(); // compilation time confirm
  ```
  `test()` in A is called. (static binding).
  
  However, if the function is not virtual, then the binding is **static**:
  ```cpp
  class A {
    public: test() {};
  }
  
  class B: public A {
    public: test() {};
  }
  
  ```
  
  ```cpp
  A* ptr = new B;
  ptr->test(); // static binding
  ```
  
  In other words, `virtual` enables **dynamic bindling**.
  
### Static (Early) Binding, Dynamic Binding

* **`Static Binding`**: Function is confirmed to be called during compilation time.

* **`Dynmaic Binding`**: Function is confirmed to be called during runtime (more expensive).
  * Can be achieved **only** through **pointer** and **reference** types

* **`Function call using object`**:
  ```cpp
  C obj_c = C;
  obj_c.test(); // static
  
  B& obj_b = obj_c;
  obj_b.test(); // dynamic
  
  ```
  
> dynamic_cast: https://stackoverflow.com/questions/2253168/dynamic-cast-and-static-cast-in-c


## Polymorphism

* P.64 is **`dynmaic binding`**, because we **don't know what type we need to deal with** during compilatiom time

:::info
### **typeid()**

* define in `<typeinfo>`
* return: `type_info` object that consist of type information.
* `==`: compare two `type_info` object
* `name()`: return the name of object
  * the `char pointer` is unique for same type

> details: https://en.cppreference.com/w/cpp/types/type_info
:::

:::spoiler Runtime typeid Example
```cpp
class A {};

class B : public A {};

int main() {
	A* ptr = new B;
	cout << typeid(*ptr).name() << endl;
}
```

Suppose we have two classes `A`, and `B`:
```cpp
	A* ptr = new B;
	cout << typeid(*ptr).name() << endl;
```

result of `typeid` is determined during compilation time:
```cpp
1A // 1 is the length of class name
```

However, if class A consist of `virtual`:
```cpp
virtual void haha() {}
```

Dynamic binding is enable, so `typeid` is determined during runtime:
```cpp
1B
```

You can compare `type_info` using `==` or using `name()`:
```cpp
typeid(*aPtr) == typeid(*aPtr) // true
typeid(*aPtr).name() == typeid(*aPtr).name() // true
```
:::

### dynamic_cast()

* only available for pointers and ref of polymorphic class (i.e. class with `virtual`)
  * it uses `typeid` to check is cast workable

* return type `T` pointer if casting success, otherwise `nullptr` is returned

### Override

* **checking** the function is really overriding virtual function or not

:::danger
```cpp
void f(int) const override; // valid
void f(int) override const; // invalid
```

The order of `const` and `override` cannot be flipped!
:::

## Virtual Destructor

* Usually be used for base class

## Abstract Base Class

* Abstract Base Class cannot be created directly
* cannot be parameter
* cannot be return type

## final

* prevent further `inheritance` / `override`
  ```cpp
  class B final: A {/***/}
  ```
  ```cpp
  class B: public A {
    public:
      void test() const override final {}
  }
  ```
  :::success
  ```cpp
  void test() const final override {}
  ```
  
  the order of `final` and `override` can be flipped. (i.e. position of `const` must be fixed)
  :::