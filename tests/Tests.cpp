#include <gtest/gtest.h>


#include <Dice/graphql-parser/internal/QueryVisitor.hpp>
#include <Dice/graphql-parser/internal/ResponseVisitor.hpp>
#include <Dice/graphql-parser/internal/SchemaVisitor.hpp>
#include <Dice/parser.hpp>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
