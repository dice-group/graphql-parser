#ifndef GRAPHQL_PARSER_GRAPHQLPARSER_HPP
#define GRAPHQL_PARSER_GRAPHQLPARSER_HPP

#include <GraphQL/GraphQLLexer.h>

#include "Dice/graphql-parser/visitor/SchemaVisitor.hpp"
#include "Dice/graphql-parser/visitor/QueryVisitor.hpp"


namespace Dice::graphql_parser {

    using TypeDefinitions = Dice::graphql_parser::internal::TypeDefinitions;

	using namespace Dice::graphql_parser::visitor;

    class GraphQLParser {

	public:

        // parses a graphql schema
        static std::shared_ptr<TypeDefinitions>
        parseSchema(const std::string &document) {
            // parse document
            antlr4::ANTLRInputStream input(document);
            Dice::graphql_parser::base::GraphQLLexer lexer(&input);
            antlr4::CommonTokenStream tokens(&lexer);
            Dice::graphql_parser::base::GraphQLParser parser(&tokens);
            auto root = parser.document();
            SchemaVisitor schema_visitor{};
            schema_visitor.visitDocument(root);
            auto type_defs = schema_visitor.getTypeDefinitions();
            return type_defs;
		}
        // parses a graphql query
        static std::shared_ptr<QueryVisitor::ParsedGraphQL>
        parseQuery(const std::string &document, const std::string &query_name = "") {
            // parse document
            antlr4::ANTLRInputStream input(document);
            Dice::graphql_parser::base::GraphQLLexer lexer(&input);
            antlr4::CommonTokenStream tokens(&lexer);
            Dice::graphql_parser::base::GraphQLParser parser(&tokens);
            auto root = parser.document();
			Dice::graphql_parser::base::GraphQLParser::OperationDefinitionContext* query;
            if (root->definition().empty())
                throw std::runtime_error("Document does not contain any definitions");
			if(query_name.empty()) {
				if(root->definition().size() > 1)
				    throw std::runtime_error("Provide a name for the query operation to be executed");
				if(not root->definition()[0]->executableDefinition() and
					not root->definition()[0]->executableDefinition()->operationDefinition())
                    throw std::runtime_error("Executable request contains does not contain executable definition");
				if(root->definition()[0]->executableDefinition()->operationDefinition()->operationType() and
                    root->definition()[0]->executableDefinition()->operationDefinition()->operationType()->getText() != "query")
                    throw std::runtime_error("Only queries are currently supported");
				query = root->definition()[0]->executableDefinition()->operationDefinition();
            }
			else {
				// make sure that the are no type definitions and no type system extensions in the document
				for(const auto &definition : root->definition()) {
					if(definition->typeSystemDefinition() or definition->typeSystemExtension())
						throw std::runtime_error("Executable request contains type system definition");
					if(definition->executableDefinition()->operationDefinition()->name()->getText() == query_name) {
                        if(definition->executableDefinition()->operationDefinition()->operationType()->getText() != "query")
                            throw std::runtime_error("Only queries are currently supported");
						query = definition->executableDefinition()->operationDefinition();
					}
				}
			}
			QueryVisitor query_visitor{};
			return query_visitor.visitOperationDefinition(query).as<std::shared_ptr<QueryVisitor::ParsedGraphQL>>();
		}

	};

} // namespace Dice::graphql_parser



#endif//GRAPHQL_PARSER_GRAPHQLPARSER_HPP
