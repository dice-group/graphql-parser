#include "Dice/graphql-parser/GraphQLParser.hpp"

namespace Dice::graphql_parser::tests::schema {

    using namespace Dice::graphql_parser;

    class TestQueryParsing : public ::testing::Test {

	protected:
		std::string query = "{"
							"  people {"
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
		assert(parsed_query->all_fields_names.size() == 1);
        assert(parsed_query->all_result_labels[0].size() == 3);
        assert(parsed_query->all_operands_labels[0].size() == 13);
		assert(parsed_query->all_fields_names[0].size() == 5);
	}

}