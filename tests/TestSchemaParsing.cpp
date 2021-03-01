#include "Dice/graphql-parser/GraphQlParser.hpp"

namespace Dice::graphql_parser::tests::schema {

	using namespace Dice::graphql_parser;

	class TestSchemaParsing : public ::testing::Test {

	protected:

		std::string schema = "type Query {\n"
							 "  people: [Person]\n"
							 "  articles: [Article]\n"
							 "  journals: [Journal]\n"
							 "}\n"
							 "\n"
							 "type Person @uri(value: \"http://xmlns.com/foaf/0.1/Person\") {\n"
							 "  name: String @uri(value: \"http://xmlns.com/foaf/0.1/name\")\n"
							 "}\n"
							 "type Article @uri(value: \"http://localhost/vocabulary/bench/Article\") {\n"
							 "  note: String @uri(value: \"http://swrc.ontoware.org/ontology#note\")\n"
							 "  pages: Int! @uri(value: \"http://swrc.ontoware.org/ontology#pages\")\n"
							 "  title: String @uri(value: \"http://purl.org/dc/elements/1.1/title\")\n"
							 "  homepage: String @uri(value: \"http://xmlns.com/foaf/0.1/homepage\")\n"
							 "  journal: Journal @uri(value: \"http://swrc.ontoware.org/ontology#journal\")\n"
							 "  creator: [Person] @uri(value: \"http://purl.org/dc/elements/1.1/creator\")\n"
							 "}"
							 "\n"
							 "type Journal @uri(value: \"http://localhost/vocabulary/bench/Journal\") {\n"
							 "  number: Int @uri(value: \"http://swrc.ontoware.org/ontology#number\")\n"
							 "  volume: Int @uri(value: \"http://swrc.ontoware.org/ontology#volume\")\n"
							 "  issued: Int @uri(value: \"http://purl.org/dc/terms/issued\")\n"
							 "  editor: [Person] @uri(value: \"http://swrc.ontoware.org/ontology#editor\")\n"
							 "}\n";

	};

	TEST_F(TestSchemaParsing, ObjectDefinitions) {
		auto type_defs = GraphQLParser::parseSchema(schema);
		assert(type_defs->object_definitions.size() == 4);
		assert(type_defs->object_definitions[0]->name == "Query");
		assert(type_defs->object_definitions[1]->name == "Person");
		assert(type_defs->object_definitions[2]->name == "Article");
		assert(type_defs->object_definitions[3]->name == "Journal");
	}

    TEST_F(TestSchemaParsing, ObjectDirectives) {
        auto type_defs = GraphQLParser::parseSchema(schema);
        assert(type_defs->object_definitions[0]->directives.empty());
        assert(type_defs->object_definitions[1]->directives.size() == 1);
		assert(type_defs->object_definitions[1]->directives[0]->name == "uri");
		assert(type_defs->object_definitions[1]->directives[0]->arguments.size() == 1);
		assert(type_defs->object_definitions[1]->directives[0]->arguments.contains("value"));
        assert(type_defs->object_definitions[1]->directives[0]->arguments["value"] == "\"http://xmlns.com/foaf/0.1/Person\"");
        assert(type_defs->object_definitions[2]->directives[0]->name == "uri");
        assert(type_defs->object_definitions[2]->directives[0]->arguments.size() == 1);
        assert(type_defs->object_definitions[2]->directives[0]->arguments.contains("value"));
        assert(type_defs->object_definitions[2]->directives[0]->arguments["value"] == "\"http://localhost/vocabulary/bench/Article\"");
    }

	TEST_F(TestSchemaParsing, FieldDefinitions) {
        auto type_defs = GraphQLParser::parseSchema(schema);
		const auto& query_type = type_defs->object_definitions[0];
		assert(query_type->field_definitions.size() == 3);
		assert(query_type->field_definitions[0]->name == "people");
		assert(query_type->field_definitions[1]->name == "articles");
		assert(query_type->field_definitions[2]->name == "journals");
        const auto& journal_type = type_defs->object_definitions[3];
        assert(journal_type->field_definitions.size() == 4);
        assert(journal_type->field_definitions[0]->name == "number");
        assert(journal_type->field_definitions[1]->name == "volume");
        assert(journal_type->field_definitions[2]->name == "issued");
        assert(journal_type->field_definitions[3]->name == "editor");
	}

    TEST_F(TestSchemaParsing, FieldDirectives) {
        auto type_defs = GraphQLParser::parseSchema(schema);
        const auto& person_type_field_defs = type_defs->object_definitions[1]->field_definitions;
		assert(person_type_field_defs[0]->directives.size() == 1);
		assert(person_type_field_defs[0]->directives[0]->name == "uri");
		assert(person_type_field_defs[0]->directives[0]->arguments.size() == 1);
		assert(person_type_field_defs[0]->directives[0]->arguments.contains("value"));
		assert(person_type_field_defs[0]->directives[0]->arguments["value"] == "\"http://xmlns.com/foaf/0.1/name\"");
    }

	TEST_F(TestSchemaParsing, FieldTypes) {
        auto type_defs = GraphQLParser::parseSchema(schema);
        const auto& query_type_field_defs = type_defs->object_definitions[0]->field_definitions;
		assert(query_type_field_defs[0]->type_name == "Person");
		assert(query_type_field_defs[0]->is_list == true);
		assert(query_type_field_defs[0]->non_null == false);
		assert(query_type_field_defs[0]->null_list_values == true);
        const auto& article_type_field_defs = type_defs->object_definitions[2]->field_definitions;
        assert(article_type_field_defs[1]->type_name == "Int");
        assert(article_type_field_defs[1]->is_list == false);
        assert(article_type_field_defs[1]->non_null == true);
        assert(article_type_field_defs[5]->type_name == "Person");
        assert(article_type_field_defs[5]->is_list == true);
        assert(article_type_field_defs[5]->non_null == false);
        assert(article_type_field_defs[5]->null_list_values == true);
	}



}
