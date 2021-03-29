#include "Dice/graphql-parser/GraphQLParser.hpp"

namespace Dice::graphql_parser::tests::schema {

    using namespace Dice::graphql_parser;

    class TestQueryParsing : public ::testing::Test {

	protected:
		std::string query = "{"
							"  people(age: 25) {"
							"    name"
							"    age"
							"    company {"
							"      location"
							"    }"
							"  }"
							"}";

	};

	TEST_F(TestQueryParsing, SimpleQuery) {
		auto parsed_query = GraphQLParser::parseQuery(query);
		assert(parsed_query->all_result_labels.size() == 1);
		assert(parsed_query->all_operands_labels.size() == 1);
		assert(parsed_query->all_fields_name_arguments.size() == 1);
        assert(parsed_query->all_result_labels[0].size() == 3);
        assert(parsed_query->all_operands_labels[0].size() == 14);
		assert(parsed_query->all_fields_name_arguments[0].size() == 6);
	}

}