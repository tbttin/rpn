# About

- By using [Shunting-yard
  algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm)
  _rpn_ can evaluate a mathematics expression, that includes:

  + Multiple digits number (floating point)

  + Opearators

  + Parentheses

  + Unary operator (-)

  Plus:

  + Invalid parentheses check

  + Invalid operators check

- Example

```console
rpn $ make run
+-22.5+-40.0 = -62.5
3+4*2/(1-5)^2^3 = 3.00012
-1+2+-1-(-3) = 3
```

# Build

- Inside the _rpn_ top level directory:

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

# What Did I Learn From Writing The rpn

- I learned how to design and construct data type like a `struct` that
  can hold both _operands_ (numbers) and _operators_ (operator ids).

- Data type (`Node`) and `Stack` are separable.

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

