#ifndef GRAPHQL_PARSER_TYPEDEFINITIONS_HPP
#define GRAPHQL_PARSER_TYPEDEFINITIONS_HPP

#include "Dice/graphql-parser/internal/Directive.hpp"
#include "Dice/graphql-parser/internal/FieldDefinition.hpp"

namespace Dice::graphql_parser::internal {

	struct ObjectDefinition;
	struct InterfaceDefinition;

	struct TypeDefinitions {
        std::vector<std::unique_ptr<ObjectDefinition>> object_definitions{};
        std::vector<std::unique_ptr<InterfaceDefinition>> interface_definitions{};
	};

    struct ObjectDefinition {
        std::string name{};
        std::vector<std::unique_ptr<Directive>> directives{};
		std::vector<std::unique_ptr<FieldDefinition>> field_definitions{};
		std::vector<std::string> implemented_interfaces{};
    };

	struct InterfaceDefinition {
        std::string name{};
        std::vector<std::unique_ptr<Directive>> directives{};
        std::vector<std::unique_ptr<FieldDefinition>> field_definitions{};
	};

} // Dice::graphql_parser::internal

#endif//GRAPHQL_PARSER_TYPEDEFINITIONS_HPP
