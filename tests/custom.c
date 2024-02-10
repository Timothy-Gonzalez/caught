// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

struct Animal
{
    char *type;
    char *name;
    size_t age;
};

typedef struct Animal Animal;

char *animal_formatter(Animal animal)
{
    char *result;
    asprintf(&result, "Animal { type = %s, name = %s, age = %zu }",
             animal.name, animal.type, animal.age);
    return result;
}

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

#define EXPECT_ANIMAL(lhs, op, rhs)                                     \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(ANIMAL, Animal, lhs, op, rhs, \
                                          animal_evaluator, animal_formatter)

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
