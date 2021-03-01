#ifndef GRAPHQL_PARSER_FIELDDEFINITION_HPP
#define GRAPHQL_PARSER_FIELDDEFINITION_HPP

#include "Dice/graphql-parser/internal/Directive.hpp"

namespace Dice::graphql_parser::internal {

    struct FieldDefinition {

        std::string name{};
		std::string type_name{};
		bool non_null = false;
		bool is_list = false;
		bool null_list_values = true;
        std::vector<std::unique_ptr<Directive>> directives{};
        std::map<std::string, std::string> arguments{};

    };

} // Dice::graphql_parser::internal

#endif//GRAPHQL_PARSER_FIELDDEFINITION_HPP
