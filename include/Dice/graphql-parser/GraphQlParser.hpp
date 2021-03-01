#ifndef GRAPHQL_PARSER_GRAPHQLPARSER_HPP
#define GRAPHQL_PARSER_GRAPHQLPARSER_HPP

#include <GraphQL/GraphQLLexer.h>

#include "Dice/graphql-parser/visitor/SchemaVisitor.hpp"
#include "Dice/graphql-parser/visitor/ResponseVisitor.hpp"
#include "Dice/graphql-parser/visitor/QueryVisitor.hpp"


namespace Dice::graphql_parser {

    class GraphQLParser {

		using TypeDefinitions = Dice::graphql_parser::internal::TypeDefinitions;

	public:

		// parses a graphql schema
		static std::shared_ptr<TypeDefinitions> parseSchema(const std::string &document) {
            // parse document
            antlr4::ANTLRInputStream input(document);
            Dice::graphql_parser::base::GraphQLLexer lexer(&input);
            antlr4::CommonTokenStream tokens(&lexer);
            Dice::graphql_parser::base::GraphQLParser parser(&tokens);
			auto root = parser.document();

			visitor::SchemaVisitor schema_visitor{};
			schema_visitor.visitDocument(root);
			return schema_visitor.getTypeDefinitions();
		}

		// parses a graphql query
		static void parseQuery(const std::string &document) {
			// parse document
            antlr4::ANTLRInputStream input(document);
            Dice::graphql_parser::base::GraphQLLexer lexer(&input);
            antlr4::CommonTokenStream tokens(&lexer);
            Dice::graphql_parser::base::GraphQLParser parser(&tokens);
            auto root = parser.document();

			if (root->definition().empty())
				throw std::runtime_error("Document does not contain any definitions");
			for (const auto &definition : root->definition()) {
				if (definition->typeSystemDefinition() or definition->typeSystemExtension())
                    throw std::runtime_error("Executable request contains type system definition");
			}
		}

	};

} // namespace Dice::graphql_parser



#endif//GRAPHQL_PARSER_GRAPHQLPARSER_HPP
