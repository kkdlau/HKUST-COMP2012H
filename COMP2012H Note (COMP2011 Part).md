**IMPORTANT**: This note summarizes COMP2011 contents with a little part of COMP2012.

# COMP2012H Note

## Include Directive

* user-defined library extension: `.h` or `.hpp` (they are the same in C++)
    ```cpp
    #include "canvas.hpp"
    #include "curve.h"
    ```
* Standard C++ library header files have **no file suffix**
* `""` tell the preprocessor that the file to be included is a **user-defined header file**, so it will
    1. look for that header file in the current folder containing our source code.
    2. If it doesn’t find the header file there, it will check any other include paths that we’ve specified in our compiler/IDE settings.
    3. If that fails, it will fall back to checking the system folders.

    so it's ok to include `iostream` like this:
    ```cpp
    #include "iostream"
    ```

    :::danger
    include standard C++ libraries using `""` will increase the compile time.
    :::

## Data Type

a simple program that prints the size of all data type:
```cpp
int main() {
    cout << "sizeof(bool) = " << sizeof(bool) << endl;
    cout << "sizeof(char) = " << sizeof(char) << endl;
    cout << "sizeof(short) = " << sizeof(short) << endl;
    cout << "sizeof(int) = " << sizeof(int) << endl;
    cout << "sizeof(long) = " << sizeof(long) << endl;
    cout << "sizeof(long long) = " << sizeof(long long) << endl;
    cout << "sizeof(float) = " << sizeof(float) << endl;
    cout << "sizeof(double) = " << sizeof(double) << endl;
    cout << "sizeof(long double) = " << sizeof(long double) << endl;
    return 0;
}
```

:::info
**sizeof**: `sizeof` can be **operator** or **keyword**. However, you don't need to include any standard library to use it.
:::

### Integer

* The size of data type depends on machines.
![](https://i.imgur.com/7y6roCP.png)

    ```cpp
    // short = short int
    // long = long int
    // long long = long long int
    sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
    ```
* integer type can be expanded using `unsigned`, `signed`:
    ```cpp
    unsigned short int var_0 = 0;
    signed long long int var_1 = 0;
    ```

### float & double

float & double can holds **very large integers** that cannot hold in `long long`.

* `float`: single-precision (1 bit for sign, 8 bits for exponent, 23 bits for mantissa)
* `double`: double-precision
* In scientific notation, a number has 2 components:
    * mantissa
    * exponent

### char

* using `\b` can delete a character
    ```cpp
    // Hello, World! is printed
    cout << "Hello, W" << "\b" << "World!" << endl;
    ```
    
### bool

* you can use `boolalpha` and `noboolalpha` to control how bool is printed
* `boolalpha` and `noboolalpha` is defined in `<ios>`

## Overflow, Underflow

* **Underflow**: when the -ve exponent becomes too large to fit in the exponent field of the floating-point number.
* **Overflow**: when the +ve exponent becomes too large to fit in the exponent field of the floating-point number.

## Coercion / Casting

* Coercion: done by compiler
* Casting: Manually done by using `static_cast<type>()`
* **Truncation** (i.e. remove all decminal places) is used in converting a
`float` | `double` to `short` | `int` | `long`.

## Constants

* Literal Constants: `1`, `2`, `3`, `'a'`, `3.0f`, `"COMP2012H"`
* Symbolic Constants **must be initialized** during definition.
* **Memory is not allocated** if literal constant is assigned to symbolic Constants.

## Logic operator

### Short-circuit evaluation

```cpp
int x = 1;
if (false && ++x);
cout << x << endl;
```

Since the result of `&&` is determinated by left operand `false`, `++x` doesn't evulate. This optimization is called **Short-circuit evaluation**.

Extra example:
```cpp
if (y != 0 && x / y == 4);
```

### `==` equality operator

* for `float` / `double`, `==` may not return proper results.
    to compare with `float` / `double`:
    ```cpp
    bool equals(const float& a, const float& b) {
        return (fabs(a - b) < numeric_limits<float>::epsilon());
    }

    bool equals(const double& a, const double& b) {
        return (fabs(a - b) < numeric_limits<double>::epsilon());
    }
    ```
* Operator Precedence and Associativity

    |       operator       | Associativity |
    |:--------------------:|:-------------:|
    |         `()`         |       /       |
    | `++`, `--`, `!`, `-` |      R2L      |
    |    `*`, `/`, `%`     |      L2R      |
    |       `+`, `-`       |      L2R      |
    | `>`, `>=`, `<`, `<=` |      L2R      |
    |      `==`, `!=`      |      L2R      |
    |         `&&`         |      L2R      |
    |         `=`          |      R2L      |

## switch

* `default` is checked after all cases are checked.

    ```cpp
    switch(x) {
        default:
            cout << "other case" << endl;
        case 1:
            cout << "1" << endl;
        case 2:
            cout << "2" << endl; break;
        case 3:
            cout << "3" << endl;
    }
    ```
    
    * if `x = 1`:
        ```cpp
        1
        2
        ```
    * if `x = 4`:
        ```cpp
        other case
        1
        2
        ```
* same actions can be shared with different cases:
    ```cpp
    switch(x) {
        // case 1 and 2 share the same action
        case 1:
        case 2:
            cout << "case 1 or 2" << endl;
            break;
        case 3:
            cout << "case 3" << endl;  
            break;
    }
    ```

## enum

* In an enum, same values can appear more than once:
    ```cpp
    enum Instructor: unsigned short {
        DESMOND = 0,
        PETER = 0,
        MARY
    };
    ```

* the enum members in fact are **symbolic constants**
* In C++, enum is a stronger type (unlike C)
    * If you assign `int` to `enum_type`, you must use `static_cast<enum_type>`
    ```cpp
    enum Blood {
        A,
        B,
        O,
        AB
    };
    
    Blood blood_type = static_cast<Blood>(0); // A
    Blood blood_type_2 = 0; // error
    ```
    * However, **implicit conversion** is done during equality comparsion (i.e. No need `static_cast`)
    ```cpp
    if (blood_type == 0); // valid statement
    ```
    
## setw

* `cout << setw(<size>)` Specifies the length of next data

## Function

### Formal Parameters, Actual Parameters

```cpp
void addition(int a, int b); // valid
void addition2(int c, d); // invalid

addition(0, 1);
```

`a` and `b` are **formal parameters**. `0` and `1` are **actual parameters**.


### Function Prototype

```cpp
int max(int, int);
```

* `int, int` is **Function Signature**.
* It's **API**
* A function may be **declared many times**


### Default Arguments

* Only can be used after formal parameters which have no deafault arguments
```cpp
int max(int a, int b = INT_MAX);  // valid
int max2(int a = INT_MAX, int b); // invalid
```

* The default arguments can be acquired from previous function declaration

```cpp
void f(int n, int k = 1);
void f(int n = 0, int k); // OK: k's default supplied
                          // by previous decl in the same scope
                          
void f(int, int);
void f(int, int k = 1);   //OK: adds a default
```

### Function Overloading

```cpp
void f(int a) {
    return;
}

void f(int& a) {
    return;
}

int a = 1;
```

This can be compiled when constant integer passes to `f()`. However, if you pass a variable to `f()`, the **compiler confuses** and don't know what one should be called. So **compile errors raise**.

### Overloading Resolution

1. Exact match
2. match after type promotion(i.e. cast the type to more general)
    * `float` -> `double`
    * `short` -> `int`
3. match after type conversion

## Reference Variable

* In function, `Pass By Reference` is less expensive than `Pass By Value`.
* Ref **must be initialized** during declaration. So:
    * It's safer then using pointer (You don't need to check is it pointing to `NULL`)
* Invalid reference exception:
    ```cpp
    // It's ok to compile since you are not access *x.
    // However, when you assign sth to y,
    // the error occurs because y references to NULL.
    int *x = 0;
    int& y = *x;
    ```

* Declaring `ref int` and `int` in a line:

    ```cpp
    int &a = b, c;
    ```
    
    a is `ref int`, and c is `int`.


## Global Variable

* When a global variable is defined, it is initialized as `0`.
* To access global scope variable, use `::`.
    ```cpp
    int a = 0;
    int main() {
        int a = 10;
        ::a = 10; // global a
        a = 10;// local a
    }
    ```

## Array

* `int a[0];` is workable
* To pass a 3d array into a function, you must specify the last 2 size:
    ```cpp
    void f(float s[][5][5]);
    ```
* `a == &a == &a[0]`, but their types aren't the same.

## Struct

* Creating a struct means that assigning a temporary memory to the struct, therefore, assigning value to unnamed struct is ok

    ```cpp
    #include <iostream>

    using namespace std;

    struct haha {
        int bruh;
    };

    int main() {
        cout << ((haha){.bruh = 10} = (haha){.bruh = 20}).bruh << endl;
        return 0;
    }
    ```

* During defining `ll_node`, you can define struct pointer of **same type**:
    ```cpp
    struct ll_node {
        int val;
        ll_node* next;
    }
    ```

    It's because the **size of `ll_node*` is defined** whatever what type of pointer it is.
    :::danger
    However, if you have member which the same type of the struct:
    ```cpp
    struct ll_node {
        int val;
        ll_node next;
    }
    ```
    That's invalid, because **the size of `ll_node` haven't undefined**.
    :::

## Pointer

* `nullptr` can be deleted, **without runtime error**.

## Class

### constructor
* Constructor is a **member function**, for initialize data member.
* Constructor has **no return type**.
* Constructor is **automatically be called** even without `()`.
    
    Example:
    ```cpp
    class Person {
        public:
            Person() {
                cout << "called constructor." << endl;
            }
    };

    int main() {
        Person p;
        return 0;
    }
    ```
    
    output:
    ```cpp
    called constructor.
    ```
    
### Array for Class

* when you create an array (i.e `Array<Class>`), default constructor is called

    ```cpp
    class Book {
        Book() {
        
        }
    }
    
    
    int main() {
        Book* test =  new Book[3];
        // test array consist of  3 books,
        // and those books are constructed!
        
        
        // when you delete test
        // destructor is called for each element
        delete[] test;
    }
    ```
    
### Forward Declaration of Class
* Class can use `forward declaration` like `function`.
* Before you use the class as a member on other class / struct, you must `define` the class

	:::danger
	```cpp
	class Cell;
	
	struct List {
		Cell* data;
		Cell c;
	}

	```
	The size of Cell is unknown, that means the size of List is unknown.
	:::
	
### Different Syntax of int initialization

```cpp
int a = 2.1; // ok
int a {2.1} // warning
int a(2.1); // same as assignment operator
```

### inline function for class
* By default, all function bodies put in class are **inline function**.
* if you split the functions to `prototype` & `definition` into **two files**, then they are not `inline`. (No use even if you put `inline`)
* But if you split it, and put in one file & add `inline` keywords, then it is `inline`.
* To conclude, inline functions **must put in one file**.
