//
// Created by fakhr on 26.02.21.
//

#ifndef GRAPHQL_PARSER_QUERYVISITOR_HPP
#define GRAPHQL_PARSER_QUERYVISITOR_HPP

#include <GraphQL/GraphQLBaseVisitor.h>

namespace Dice::graphql_parser::internal {

    class QueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

    public:
        antlrcpp::Any visitDocument(base::GraphQLParser::DocumentContext *ctx) override {
           for(auto definetion : ctx->definition())
           {

           }
        }

        antlrcpp::Any visitExecutableDefinition(base::GraphQLParser::ExecutableDefinitionContext *ctx) override {
            return GraphQLBaseVisitor::visitExecutableDefinition(ctx);
        }
    };
}


#endif //GRAPHQL_PARSER_QUERYVISITOR_HPP
