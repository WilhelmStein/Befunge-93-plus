# Befunge-93+
An indirectly threaded vm for the language Befunge-93+, written in C++.

### What is Befunge-93+ ?
---

Befunge-93+ is an extension of the language [Befunge-93+](https://catseye.tc/view/Befunge-93/doc/Befunge-93.markdown), that adds a garbage collector and the following commands:

* `(c) cons` : Removes 2 elements `b`, `a` from the stack and creates a new pair (cons cell) that contains `a` in the first position and `b` in the second. Said pair's address is placed at the top of the stack.

* `(h) head` : Removes the element currently residing at the top of the stack, which must be the address of a cons cell, and puts the value in the first position of the pair at the top of the stack.

* `(t) tail` : Same as `h`, but for the second position of the pait.

