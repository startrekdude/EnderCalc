# EnderCalc Dev Docs

A quick look at what goes in to making a calculator. Organized as a tour through the code, with subsequent modules building on previous ones.

## Vector

(File [vector.c](src/vector.c))

EnderCalc manipulates expressions as a *vector* of *tokens*⁠—worry about tokens later, let's figure out the vector part. A vector is a contiguously allocated list of elements, the trick being you can add and remove elements without worrying about memory allocation. Most languages ship with one of these; in the prototype design I used a Python `list`. C doesn't, of course, so I made one. I'm very happy with the code quality of `vector.c` and might re-use it in other projects.

## Expressions

(File [expr.c](src/expr.c))

- An *expression* is a vector of *tokens*.
- A *token* is a [tagged pointer](https://en.wikipedia.org/wiki/Tagged_pointer), the tag is the type and the rest is a pointer type or a *symbol*.
  - My particular implementation of tagged pointers is for amd64 and required no allocator changes. An alternate implementation may need to be used for ports.
- *Symbols* are constants, `#define`d from the void and imbued with meaning.

Let's double click. The types are:

- `TOKEN_EXPR` - the token is a pointer to a subexpression. Used for brackets: `3 * (5 + 2).`
- `TOKEN_STR` - the token is a pointer to a string slice.
- `TOKEN_NUM` - the token is a pointer to a big float number (a `bf_t`, if you really care).
- `TOKEN_SYM` - last but certainly not least, the token is a symbol (**not** a pointer at all). `ADD`, `ANS`, `ATANH` are all symbols.

I started with routines for printing and freeing expressions. Debuggability and memory management are very important considerations in C99.

## Parser

(File [parse.c](src/parse.c), parse_*.c)

The parser's job is to turn a string, like `"sin pi (34 ) +   -  5(2*2)"`, into an expression, like `SIN 3.14 MUL (34) ADD NEG 5 MUL (2 MUL 2)`. The second is, obviously (at least if you're a programmer), much easier to evaluate than the first. The parser is written as a sequence of passes that operate on expressions and emit expressions (except the first one, which operates on a string).

Let's look over the passes. I'll use `((3^4)(e^-3))/4!` as an example.

1. `match_brackets`: convert a string into a expression composed entirely of subexpressions and string slices. This can fail if there are too many opening or closing braces. It's hard to demonstrate visually, but the output is `((3^4) (e^-3)) /4!`.
2. `tokenize`: recursively split any string slice token into a series of number and symbol tokens. This can fail if the user enters input not composed of valid symbols. This step ignores whitespace. Output: `((3 POW 4)) (E POW SUB 3)) DIV 4 FAC`.
3. `constants`: recursively replace constant symbols with number constants. Output: `((3 POW 4) (2.7182 POW SUB 3)) DIV 4 FAC`. (the astute among you have noted that I could've used `exp-3` instead of `e^-3`. this is, of course, correct—but not useful for the demo)
4. `implicit_multply`: recursively add implicit multiplication operators; for example `4(3)` should be `4 MUL (3)`. The rule used here is "replace every subexpression preceded by a subexpression, number, postfix operator symbol, or non-operator symbol with a `MUL` symbol followed by that subexpression". Bit of a mouthful. Output: `((3 POW 4) MUL (2.7182 POW SUB 3)) DIV 4 FAC`.
5. `unary_neg`: recursively replace `SUB` with `NEG` in situations where `SUB` isn't valid. Basically, `4 + -3` should be 1 and not a syntax error—we need to disambiguate between `-` as `SUB` and `-` as `NEG`. The rule used here is "replace `SUB` with `NEG` when it is preceded by nothing or a operator symbol that is not unary postfix". Output: `((3 POW 4) MUL (2.7182 POW NEG 3)) DIV 4 FAC`.

## Evaluator

The evaluator accepts a parsed expression and tried to evaluate it to a single numerical answer. There are a myriad of ways this can go wrong—for example, `ADD 2` is not an evaluable expression (missing argument). The design is fairly simple, but first, a quick tangent:

### Argument Resolver

`resolve_arg` attempts to coerce a token into a number than can be used as input to an operator.

1. If the token is of type `TOKEN_NUM`, return it.
2. If the token is of type `TOKEN_EXPR`, evaluate it and return the result.
3. If the token if of type `TOKEN_SYM`:
   1. If the token is `SYM_ANS`, return the saved last answer.
   2. If the token is `SYM_RCLx`, return a value from memory.
   3. Otherwise, error.

Okay, now that we've got that out of the way, onwards to the evaluator proper. The evaluator first attempts to reduce the expression to a single value. It does so using the following algorithm:

- Find the next operator to execute, using precedence and associativity rules. The operator with the lowest precedence wins.
- Collect the arguments for the operator using `resolve_arg`.
- Perform the operation proper using a function pointer.
- Return the results to the expression.

(for example, `3 ADD 2 MUL 4` will be reduced to `3 ADD 8` then `11`)

Once a single value is left, it is resolved and returned.

