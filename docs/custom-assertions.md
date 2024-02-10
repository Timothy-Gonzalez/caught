# Custom Assertions

You can define your own custom assertions, for when we don't include one or you are working with custom types.

In Caught, assertions require four things:
- A type
- A formatter
- A evaluator
- A macro that uses all of the above

So, let's walk through an example. Say we have a `Animal` struct:

```c
struct Animal
{
    char *type;
    char *name;
    size_t age;
};

typedef struct Animal Animal;
```

We want to be able to do `EXPECT_ANIMAL(one, ==, another)`.

Next, we need a formatter.

## Formatters

Formatters are used to provide a string output. For our animal, this might look something like:
```
PASS ./tests/custom.c:62:
    EXPECT_ANIMAL( zebra == other_zebra )
        expected: Animal { type = Stripes, name = Zebra, age = 20 }
        to be:    Animal { type = Stripes, name = Zebra, age = 20 }
```

Caught can't automatically determine what you want to output, so we have to define a formatter.

Formatters take in the custom type, and return a string representation of it:
```c
char *animal_formatter(Animal animal)
{
    char *result;
    asprintf(&result, "Animal { type = %s, name = %s, age = %zu }",
             animal.name, animal.type, animal.age);
    return result;
}
```

**Note:** the returned string should be allocated so it can be free later. Here, we use `asprintf` to do that.

Next, we need an evaluator.

## Evaluators

Evaluators are what determine if an assertion passes or fails.
This gives you full control over how you want two things to be compared.

Formatters take in an lhs (actual), operator, and rhs (expected).

If we wanted a simple evaluator that just compares based on strict equality, we could do:

```c
bool animal_evaluator(Animal lhs, enum caught_operator operator, Animal rhs)
{
    switch (operator)
    {
    case CAUGHT_OP_EQUAL:
        return (strcmp(lhs.type, rhs.type) == 0) &&
               (strcmp(lhs.name, rhs.name) == 0) &&
               lhs.age == rhs.age;
    case CAUGHT_OP_NOT_EQUAL:
        return (strcmp(lhs.type, rhs.type) != 0) ||
               (strcmp(lhs.name, rhs.name) != 0) ||
               lhs.age != rhs.age;
    default:
        fprintf(stderr, "Invalid operator %s!\n", caught_operator_to_str(operator));
        exit(1);
    }
}
```

It's recommended to throw an error if an invalid operator that your evaluator doesn't support is used,
so that it's easy to see if something is failing.

Note that you could modify this evaluator to just require the same name, or just the same type of Animal if you wanted to.

Now that we have the three parts required, we can make our macro.

## Expect Macro

To wrap our type, formatter, and evaluator together, we can use a macro definition.

All we have to do is pass the right arguments to one of
Caught's internal expect handles (`CAUGHT_INTERNAL_EXPECT_HANDLE*`),
and Caught will take care of the rest.

For a generic assertion with just one unique type, doing so would look like this:

```c
#define EXPECT_ANIMAL(lhs, op, rhs)                                     \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(ANIMAL, Animal, lhs, op, rhs, \
                                          animal_evaluator, animal_formatter)
```

Here, we pass our function's postfix (the part after expect, in this case `ANIMAL`),
the type, lhs & op & rhs, our evaluator, and finally our formatter.

> To see more of the possible expect handles you can use if you need more customization, see [`assertion-macros.h`](../src/assertion-macros.h).

And that's it! Now, you can use this assertion in tests:

```c
TEST("custom - animal")
{
    Animal zebra = {
        .type = "Zebra",
        .name = "Stripes",
        .age = 20,
    };
    Animal other_zebra = {
        .type = "Zebra",
        .name = "Stripes",
        .age = 20,
    };
    Animal giraffe = {
        .type = "Giraffe",
        .name = "Neck",
        .age = 32,
    };

    EXPECT_ANIMAL(zebra, ==, other_zebra);
    EXPECT_ANIMAL(zebra, !=, giraffe);
}
```

For reference, you can find the full example in [this sample test](../tests/custom.c).
