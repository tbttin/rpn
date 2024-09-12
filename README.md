# About

- By using [Shunting-yard
  algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm)
  _rpn_ can evaluate a mathematics expression, that includes:

  + Multiple digits number (floating point)

  + Opearators (+, -, unary -, unary +, *, /)

  + Parentheses

  Plus:

  + Invalid parentheses check

  + Invalid operators check

# Build

- On Linux (Arch Linux), inside the _rpn_ top level directory:

```shell
make
```

## Run

```shell
./build/rpn
```

or

```shell
make run
```

# TODO

- [ ] Optional multi-threads.

- [x] Support unary minus operator.

  + Redesign unary relations?

- [ ] Add debugging utilities.

  + [ ] Command line args support.

  + [ ] Enable/disable debug with command line option or environment
    variables.

  + [ ] Debug level as a flag to enable debugging a specific function.

  + [ ] Log file.

- [ ] Implicit multiplication

  + 2(3+4)5

  + (1+2)(2+3)

  + Whenever there is no operator, it is multiplication?

- [ ] Support function operators (sin, cos, .etc)?

- [ ] Support portability.

- [ ] A development branch.


# What Did I Learn From Writing The rpn

- I learned how to design and construct data type like a `struct` that
  can hold both _operands_ (numbers) and _operators_ (operator ids).

- Data type (`Node`) and `Stack` are separable.

  + `stack.h` can be use as a library.

- Yeah, I decided to avoid **dynamic** memory allocation, it require
  a lot of (multi-layers) memory management.

  I chose to stick with **automatic** memory allocation and **static**
  memory allocation, they are simple.

- Operation orders in mathematics and in computer are not the same,
  there is no "standard rule".

  But there might have one: whenever it is ambiguous, just use
  parentheses.

- Working with mathematics expression, it reinforces my mathematics area
  a little bit (it was really weak already).

