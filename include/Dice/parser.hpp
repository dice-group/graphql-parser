//
// Created by fakhr on 26.02.21.
//

#ifndef GRAPHQL_PARSER_PARSER_HPP
#define GRAPHQL_PARSER_PARSER_HPP

#include <GraphQL/GraphQLLexer.h>

#include "Dice/graphql-parser/internal/SchemaVisitor.hpp"
#include "Dice/graphql-parser/internal/ResponseVisitor.hpp"
#include "Dice/graphql-parser/internal/QueryVisitor.hpp"


namespace Dice::graphql_parser {


    std::vector<std::string> parseSchema(std::string document) {
        antlr4::ANTLRInputStream input(document);
        Dice::graphql_parser::base::GraphQLLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        Dice::graphql_parser::base::GraphQLParser parser(&tokens);

        auto tree = parser.document();

        Dice::graphql_parser::internal::SchemaVisitor visitor;
        return visitor.visitDocument(tree);
    }

    std::vector<std::string> parseQuery(std::string document) {
        antlr4::ANTLRInputStream input(document);
        Dice::graphql_parser::base::GraphQLLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        Dice::graphql_parser::base::GraphQLParser parser(&tokens);

        auto tree = parser.document();

        Dice::graphql_parser::internal::QueryVisitor visitor;
        return visitor.visitDocument(tree);
    }


}// namespace Dice::graphql-parser



#endif //GRAPHQL_PARSER_PARSER_HPP
