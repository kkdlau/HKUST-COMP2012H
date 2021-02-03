# COMP2012H - AVL

Note: https://course.cse.ust.hk/comp2012h/notes/20-avl-full.pdf

## What is AVL

* Height difference is **at most 1**
  * If left_node = `nullptr`, assume left tree height = `-1`
* is BST


## Tree Balance

* Only need to handle the most nested problem node
* Rotation (Consider **first two** Edge from Problem Node to Trouble Node):
  * LL: Start from Problem Node, rotate **RIGHT**
  * RR: Start from Problem Node, rotate **LEFT**
  * RL: Start from the below of Problem Node, rotate to **RIGHT**, then Problem Node roate to **LEFT**
: Start from the below of Problem Node, rotate to **LIFT**, then Problem Node roate to **RIGHT**


:::danger
If during rotation, there is no node cannt be inserted, then insert that on the node the to be replaced 
:::