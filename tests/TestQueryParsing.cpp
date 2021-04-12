#include "Dice/graphql-parser/GraphQLParser.hpp"

namespace Dice::graphql_parser::tests::schema {

	std::string operands_labels_to_str(const std::vector<std::vector<char>> &operands_labels) {
		std::string operands_labels_str{};
		for (auto &operand_labels : operands_labels) {
			for (auto label : operand_labels) {
				operands_labels_str.push_back(label);
			}
			operands_labels_str.push_back(',');
		}
		operands_labels_str.erase(operands_labels_str.end()-1); // remove trailing comma
		return operands_labels_str;
	}

    std::string result_labels_to_str(const std::vector<char> &result_labels) {
        std::string result_labels_str{};
		for (auto label : result_labels)
			result_labels_str.push_back(label);
        return result_labels_str;
    }

	TEST(QueryParsing, SimpleQuery) {
        std::string query = "{"
                            "  people(age: 25) {"
                            "    name"
                            "    age"
                            "    company {"
                            "      location"
                            "    }"
                            "  }"
                            "}";
		auto parsed_query = GraphQLParser::parseQuery(query);
		std::string actual_operands_labels = "a,a,[,ab,],[,ac,],[,ad,[,de,],]";
		std::string actual_result_labels = "abcde";
		assert(parsed_query->all_result_labels.size() == 1);
		assert(parsed_query->all_operands_labels.size() == 1);
		assert(parsed_query->all_fields_name_arguments.size() == 1);
        assert(result_labels_to_str(parsed_query->all_result_labels[0]) == actual_result_labels);
        assert(operands_labels_to_str(parsed_query->all_operands_labels[0]) == actual_operands_labels);
		assert(parsed_query->all_fragment_labels[0].empty());
		assert(parsed_query->all_fields_name_arguments[0].size() == 6);
	}

	TEST(QueryParsing, InlineFragment) {
        std::string query = "{"
                            "  animal {"
                            "    name"
                            "    ... on Dog {"
                            "      tailLength"
                            "    }"
                            "    ... on Bird {"
							"      wingLength"
							"    }"
                            "  }"
                            "}";
        auto parsed_query = GraphQLParser::parseQuery(query);
        std::string actual_operands_labels = "a,[,ab,],[,a,[,ac,],],[,a,[,ad,],]";
        std::string actual_result_labels = "abcd";
        assert(parsed_query->all_result_labels.size() == 1);
        assert(parsed_query->all_operands_labels.size() == 1);
        assert(parsed_query->all_fields_name_arguments.size() == 1);
        assert(result_labels_to_str(parsed_query->all_result_labels[0]) == actual_result_labels);
        assert(operands_labels_to_str(parsed_query->all_operands_labels[0]) == actual_operands_labels);
		assert(parsed_query->all_fragment_labels[0].size() == 2);
		assert(parsed_query->all_fragment_labels[0].contains('c') and parsed_query->all_fragment_labels[0].contains('d'));
        assert(parsed_query->all_fields_name_arguments[0].size() == 6);
	}

    TEST(FieldCollection, LeafFieldCollection) {
        std::string query = "{"
                              "  people {"
                              "    name"
                              "    age"
                              "    age"
                              "  }"
                              "}";
        auto parsed_query = GraphQLParser::parseQuery(query);
        std::string actual_operands_labels = "a,[,ab,],[,ac,]";
        std::string actual_result_labels = "abc";
        assert(parsed_query->all_result_labels.size() == 1);
        assert(parsed_query->all_operands_labels.size() == 1);
        assert(parsed_query->all_fields_name_arguments.size() == 1);
        assert(result_labels_to_str(parsed_query->all_result_labels[0]) == actual_result_labels);
        assert(operands_labels_to_str(parsed_query->all_operands_labels[0]) == actual_operands_labels);
        assert(parsed_query->all_fields_name_arguments[0].size() == 3);
    }

    TEST(FieldCollection, InnerFieldCollection) {
        std::string query = "{"
                            "  people {"
                            "    name"
							"    company { name }"
                            "    age"
                            "    company { location }"
                            "  }"
                            "}";
        auto parsed_query = GraphQLParser::parseQuery(query);
        std::string actual_operands_labels = "a,[,ab,],[,ac,[,cd,],[,ce,],],[,af,]";
        std::string actual_result_labels = "abcdef";
        assert(parsed_query->all_result_labels.size() == 1);
        assert(parsed_query->all_operands_labels.size() == 1);
        assert(parsed_query->all_fields_name_arguments.size() == 1);
        assert(result_labels_to_str(parsed_query->all_result_labels[0]) == actual_result_labels);
        assert(operands_labels_to_str(parsed_query->all_operands_labels[0]) == actual_operands_labels);
        assert(parsed_query->all_fields_name_arguments[0].size() == 3);
    }

}