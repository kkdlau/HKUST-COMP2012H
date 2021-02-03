# COMP2012H - Initialization, Constructor & Destructor

Note: https://course.cse.ust.hk/comp2012h/notes/11-constructor-destructor-4page.pdf

Table of Content
[TOC]

## Initialization

* `{}` can be used for initialization **if and only if all data memebers** are **public**

* Example:
  ```cpp
  class Test {
    public: int a; const char* str;
  }
  
  int main() {
    Test t{10, "ok"}; // behave like struct
  }
  ```

## Constructor

* No return type (not even `void` type)
* Name must be the same as class
* constuctor can be called in other constructor
* We can overlord constructor in order to define other types of constructor

## Default constructor

* A function without passing any parameter.

  ```cpp
  // assume within class scope
  Test(int n = 10) {};
  Test() {};
  ```
    
* If no default constructor defined, then **compiler generates one**
  * The constructor is empty function body => **does nothing**
  * the purpose is to **reserve enough space for data member**
* Be generated when no other constructor are defined 

* `= deafult`(Since C++11): explicitly generate
  * Usually be used when you want default constructor when you define other types of constructors 
  ```cpp
  Word() = default;
  ```
* `= delete`(Since C++11): explicitly don't generate
  ```cpp
  // don't define default copy constructor
  Word(const Word& w) = delete;
  ```

## Conversion constructor

* Meaning: convert the parameter to a class
 
* A function that can pass **one value only** to it
  ```cpp
  Test(int n);
  Test(int n, int k = 10);
  ```

* conversion constructor can be called in severed ways:
  ```cpp
  Test my_class_1(1);
  Test my_class_2 = 1;
  Test my_class_3{3}; // since C++ 11
  ```
  
* assignment operator is **implicit**, and `()` `{}` is **explicit**

* Using assignment operator is not recommended (**easy to mix up** with normal assignment!)

### Implicit Conversion

```cpp
class Word {
  private:
    int frequency;
    char* str;

  public:
    Word(char c) {/***/}
    Word(const char* s) {/***/}
    void print() const { /***/ }
};

void print_word(Word x) { x.print(); }

int main() {
  print_word("Titanic"); // implicit conversion
  print_word('A'); // implicit conversion
  return 0;
}
```

cpp considers the above statement is valid, because `char` and `char array` can be converted to `Word`.


So the following is what cpp does:
1. Instantiate a `temp` object `Word` with `char` / `char array`
2. Copy `temp` to `x`
3. Free `temp`
4. After finishing executing `print_word`, free `x`

### Avoid Implicit Conversion

* add `explicit` before the constructor
* `explicit` avoid creating object via `=` (`=` is implicit)

  ```cpp
  class Test {
    public:
      explicit Test(int n) {}
  }
  ```

* In the example above, actually 2 objects are created (move constructor)
  * It's because of compiler omptimization
  * `temp` is used as `x`


## Copy constructor

* Copy object
* It's recommended to define copy constructor **when data memebrs are pointers**

  ```cpp
  class Test() {
    private: int a; int b;
    public:
      Test(int a) {this->a = a; this->b = b;}
      Test(const Test& t) {
        a = t.a;
        b = t.b;
      }
  }
  ```
  
  :::danger
  `Test(Test& t)` is valid, but this is **not safe**.
  `Test(Test t)` leads to compiler error, because **copy constructor is not defined** (or copy constructor is called infinitely).
  :::
  
* copy constructor can be implicit / explicit (i.e. `=` is workable if no `explicit` keyword)
* Usually it is called when:
  * pass to function by value
  * initialization by `=`
  * return by value

* Example:

  ```cpp
  class Test() {/***/}

  Test cool(Test t) {
    return t; // copy constructor: 3 (copy t to return object temp)
  }

  int main() {
    Test a; // default constructor
    Test b(a); // copy constructor: 1

    Test c = cool(b); // copy constructor: 2 (copy b to t)
                      // copy constructor 4: copy temp to c
  }
  ```
  
  * total 4 times of copy constructor is called
  * But compiler does optimization
    * `temp` becomes `c`
  * flag - `fno-elide-constructors`: no constructor omptimization


### Default Copy Constructor

* memberwise copy
  * calling copy constructor for each data member

  ```cpp
  class A {
    public:
      A() {/***/}
  }
  
  int main() {
    A obj1; // default constructor
    A obj2(obj1); // default copy constructor
  }
  ```
  
* It usually cause problems when data members are pointers
  * If the class defines destructor
  * the memory pointed by data members may be **free twice**!


## Default Assignment Operator

* `=` is defined for any class if no `operator=` is defined
* What is does:
  * Memberwise copy **even copy constructor is defined by user**
  * **Array** members **are deep clone**
  * **Pointer members** perform **shallow copy**
* But for other operators, they don't be defined
* It **doesn't call** if that is **object initialization**:
  ```cpp
  Word x;
  Word y;
  Word z = x; // copy constructor
  y = z; // default assignment operator
  ```
  
## Default value for constructor

* It is recommended to put default in `.h` not `.cpp` 


## Member Initializer List
* Available for constructor only

* Order doesn't matter

* When cpp initialize data member **respectively** before constructor, it search initialization in MIL 

* `{}` / `()` both ok

* Example
  * resolve name conflict
  ```cpp
  class Test {
    private: int a; int b;
    public:
      Test(int a, int b): a(a), b(b) {
      
      }
  }
  ```
  * initialize `const` / `reference` members
  ```cpp
  class Test {
    private: const int a; int& hehe;
    public:
      Test(int a, int b): a(a), hehe(b) {
      /**
       * when program enters to function body, 
       * a is created and be constant already!
       */
      a = 1; // invalid
      }
  }
  ```
  
  * Create object instance before constructor / pick constructor to call
  ```cpp
  class A {
    private: int a = 10;
    public:
      A() {}
      A(int a): a{a} {}
      A(const A& a) {}
  }
  
  class APair {
    private: A a1; A a2;
    public:
      APair(int x, int y): a1{x}, a2{y} {}
  }
  ```
  :::info
  ### Creating instances after constructor
    ```cpp
  class A {
    private: int a;
    public:
      A() {}
      A(int a): a{a} {}
      A(const A& a) {}
  }
  
  class APair {
    private: A a1; A a2;
    public:
      APair(int x, int y) {
        a1 = x;
        a2 = y;
      }
  }
  
  int main() {
    APair test{10, 20};
  }
  ```
  Before enter the constructor, **two instances** are created for `a1` and `a2`.
  
  > default constructor calls twice.

  Now the program enters function body.
  
  First, since conversion constructor implies `int` can be converted to `A`, **a `temp` object is created by `x`**.
  
  > conversion constructor calls once.

  And then `=` performs **memberwise assignment** to `a1`.
  
  Finally the `temp` object is destroyed.
  
  > destructor calls once.
  :::
  
  :::danger
  If `const` / `ref` member doesn't initialize after constructor, then compiler will raise errors.
  :::
  
## Default Destructor

* does nothing
* After destructor, all data members are free (**free order: last in first out**)
  :::danger
  **Objects pointed by data members** won't be deleted
  :::
  
> Note P.52 and 54 are useful.