#ifndef GRAPHQL_PARSER_DIRECTIVE_HPP
#define GRAPHQL_PARSER_DIRECTIVE_HPP

namespace Dice::graphql_parser::internal {

	struct Directive {

		std::string name{};
		std::map<std::string, std::string> arguments{};

	};

} // Dice::graphql_parser::internal

#endif//GRAPHQL_PARSER_DIRECTIVE_HPP
