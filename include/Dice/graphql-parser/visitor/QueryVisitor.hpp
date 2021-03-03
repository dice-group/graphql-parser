#ifndef GRAPHQL_PARSER_QUERYVISITOR_HPP
#define GRAPHQL_PARSER_QUERYVISITOR_HPP

#include <GraphQL/GraphQLBaseVisitor.h>

namespace Dice::graphql_parser::visitor {

    class QueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

		using OperandLabels = std::vector<char>;
		using OperandsLabels = std::vector<OperandLabels>;
		using ResultsLabels = std::vector<char>;
		using FieldName = std::string;
		using FieldsNames = std::vector<FieldName>;

    public:

        // contains the information gathered from the input query
        struct ParsedGraphQL {
            // subscript operands
            std::vector<OperandsLabels> all_operands_labels{};
            // subscript result
            std::vector<ResultsLabels> all_result_labels{};
            // list of fields names
            std::vector<FieldsNames> all_fields_names{};
        };

	private:

		std::shared_ptr<ParsedGraphQL> parsed_query = std::make_shared<ParsedGraphQL>();
		// active parent field
		std::string parent_field{};
		// active subscript label
		char next_label = 'a';
		// subscript label of last visted field
		char field_label;
		// stack - subscript label of last visited selection set
		std::vector<char> selection_set_label{};

	public:

		antlrcpp::Any visitOperationDefinition(base::GraphQLParser::OperationDefinitionContext *ctx) override {
			// iterate over all root fields
			for(const auto &root_field : ctx->selectionSet()->selection()) {
				assert(root_field->field());
				field_label = next_label;
                parsed_query->all_operands_labels.emplace_back(OperandsLabels());
                parsed_query->all_operands_labels.back().emplace_back(OperandLabels{field_label});
                parsed_query->all_result_labels.emplace_back(ResultsLabels());
                parsed_query->all_fields_names.emplace_back(FieldsNames ());
                parsed_query->all_fields_names.back().emplace_back(FieldName(root_field->field()->name()->getText()));
				if(root_field->field()->selectionSet())
					visitSelectionSet(root_field->field()->selectionSet());
			}
            return parsed_query;
		}

		antlrcpp::Any visitSelectionSet(base::GraphQLParser::SelectionSetContext *ctx) override {
			selection_set_label.push_back(next_label);
			for(const auto &selection : ctx->selection()) {
				if(selection->field())
					visitField(selection->field());
			}
			selection_set_label.pop_back();
            return nullptr;
		}

		antlrcpp::Any visitField(base::GraphQLParser::FieldContext *ctx) override {
			field_label = ++next_label;
			const auto &field_name = ctx->name()->getText();
			// beginning of optional part
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{'['});
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{selection_set_label.back(), field_label});
            parsed_query->all_fields_names.back().emplace_back(FieldName(field_name));
			// leaf field - add result label
			if(not ctx->selectionSet())
                parsed_query->all_result_labels.back().push_back(field_label);
			else
				visitSelectionSet(ctx->selectionSet());
			// end of optional part
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{']'});
			return nullptr;
		}
	};

}


#endif //GRAPHQL_PARSER_QUERYVISITOR_HPP
