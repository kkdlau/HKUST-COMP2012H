# COMP2012H - Hashing

Note: https://course.cse.ust.hk/comp2012h/notes/21-hashing-full.pdf

[TOC]

## Pros

* fast for
  * search
  * insertion
  * deletion


## Cons

* Slow for:
  * find min, max
  * find value within a range


## Hash function

$h(k) = k \mod m$

* $m$ should not be:
  * power of 10 (it makes the keys consider the remaining few digit)
  * power of 2 (it makes the keys consider the remaining binary digit)

* $m$ should be **prime number**

## Collision Handling

### Separate Chaining

* For different keys but same slots => use link list to hande
  * new item can insert at the beginning of link list


### Open Addressing

#### Linear Probing
* if the slot is occupied
  * find a new slot


:::danger
All items are put together, so collision is more easier to happen, and 
:::