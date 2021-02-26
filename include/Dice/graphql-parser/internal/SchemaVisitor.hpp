//
// Created by fakhr on 26.02.21.
//

#ifndef GRAPHQL_PARSER_SCHEMAVISITOR_HPP
#define GRAPHQL_PARSER_SCHEMAVISITOR_HPP

#include <GraphQL/GraphQLBaseVisitor.h>

namespace Dice::graphql_parser::internal {

    class SchemaVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

    public:
        antlrcpp::Any visitDocument(base::GraphQLParser::DocumentContext *ctx) override {
            std::vector<std::string> f;
            for(auto definetion : ctx->definition())
            {
                if(definetion->typeSystemDefinition()!= nullptr)
                {
                    //do something
                    f.push_back(visitTypeSystemDefinition(definetion->typeSystemDefinition()));
                }
            }
        }

        antlrcpp::Any visitTypeSystemDefinition(base::GraphQLParser::TypeSystemDefinitionContext *ctx) override {
            return  std::string("hello");
        }

        antlrcpp::Any visitExecutableDefinition(base::GraphQLParser::ExecutableDefinitionContext *ctx) override {
            return GraphQLBaseVisitor::visitExecutableDefinition(ctx);
        }
    };
}


#endif //GRAPHQL_PARSER_SCHEMAVISITOR_HPP
