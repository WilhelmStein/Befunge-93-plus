# Befunge-93+
An indirectly threaded vm for the language Befunge-93+, written in C++.

### What is Befunge-93+ ?
---

Befunge-93+ is an extension of the language [Befunge-93+](https://catseye.tc/view/Befunge-93/doc/Befunge-93.markdown), that adds a garbage collector and the following commands:

* `(c) cons` : Removes 2 elements `b`, `a` from the stack and creates a new pair (cons cell) that contains `a` in the first position and `b` in the second. Said pair's address is placed at the top of the stack.

* `(h) head` : Removes the element currently residing at the top of the stack, which must be the address of a cons cell, and puts the value in the first position of the pair at the top of the stack.

* `(t) tail` : Same as `h`, but for the second position of the pait.

### Notes
---

Use the `makefile` to create the executable. You can use the `run.sh` to automatically run tests on this executable and cross check the output with the [original one](https://github.com/catseye/Befunge-93). Just clone the original Befunge-93 git inside the root folder of this project and use its makefile.

