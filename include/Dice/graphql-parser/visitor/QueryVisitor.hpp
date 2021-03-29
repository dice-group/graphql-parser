#ifndef GRAPHQL_PARSER_QUERYVISITOR_HPP
#define GRAPHQL_PARSER_QUERYVISITOR_HPP

#include <any>
#include <variant>

#include <GraphQL/GraphQLBaseVisitor.h>

namespace Dice::graphql_parser::visitor {

    class QueryVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

		using OperandLabels = std::vector<char>;
		using OperandsLabels = std::vector<OperandLabels>;
		using ResultsLabels = std::vector<char>;
		using FieldName = std::string;
		using Argument = std::pair<FieldName, std::any>;
		using FieldsNameArguments = std::vector<std::variant<FieldName, Argument>>;
		using Path = std::vector<std::pair<char, std::string>>;
		using Paths = std::vector<Path>;

    public:

        // contains the information gathered from the input query
        struct ParsedGraphQL {
            // subscript operands
            std::vector<OperandsLabels> all_operands_labels{};
            // subscript result
            std::vector<ResultsLabels> all_result_labels{};
            // list of fields names
            std::vector<FieldsNameArguments> all_fields_name_arguments{};
			// list of paths
			std::vector<Paths> all_paths{};
        };

	private:

		std::shared_ptr<ParsedGraphQL> parsed_query = std::make_shared<ParsedGraphQL>();
		// active path
		Path active_path{};
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
                active_path.push_back({field_label, root_field->field()->name()->getText()});
                parsed_query->all_operands_labels.emplace_back(OperandsLabels());
                parsed_query->all_operands_labels.back().emplace_back(OperandLabels{field_label});
                parsed_query->all_result_labels.emplace_back(ResultsLabels());
                parsed_query->all_fields_name_arguments.emplace_back(FieldsNameArguments());
                parsed_query->all_fields_name_arguments.back().emplace_back(FieldName(root_field->field()->name()->getText()));
				parsed_query->all_paths.emplace_back(Paths());
				// parse arguments of root field
				if(root_field->field()->arguments())
					visitArguments(root_field->field()->arguments());
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
			active_path.push_back({field_label, field_name});
			// beginning of optional part
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{'['});
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{selection_set_label.back(), field_label});
            parsed_query->all_fields_name_arguments.back().emplace_back(FieldName(field_name));
			// leaf field - add result label
			if(not ctx->selectionSet()) {
				parsed_query->all_result_labels.back().push_back(field_label);
				parsed_query->all_paths.back().emplace_back(active_path);
			}
			else {
				// visit arguments
				if(ctx->arguments())
					visitArguments(ctx->arguments());
				// visit nested fields
				visitSelectionSet(ctx->selectionSet());
			}
			// end of optional part
            parsed_query->all_operands_labels.back().emplace_back(OperandLabels{']'});
			// remove field name from path
			active_path.pop_back();
			return nullptr;
		}

		antlrcpp::Any visitArguments(base::GraphQLParser::ArgumentsContext *ctx) override {
			for(const auto &arg : ctx->argument()) {
				// add operand labels
				parsed_query->all_operands_labels.back().emplace_back(OperandLabels{field_label});
                auto name = arg->name()->getText();
				std::any value;
				if(arg->value()->stringValue())
					value = arg->value()->stringValue()->getText();
				else if(arg->value()->intValue())
                    value = std::stoi(arg->value()->intValue()->getText());
                else if(arg->value()->floatValue())
                    value = std::stof(arg->value()->floatValue()->getText());
				else if(arg->value()->booleanValue())
					value = arg->value()->booleanValue()->getText() == "true";
				else
					throw std::runtime_error("Non scalar values for arguments are not supported");
                parsed_query->all_fields_name_arguments.back().emplace_back(Argument(name, value));
			}
			return nullptr;
		}


	};

}


#endif //GRAPHQL_PARSER_QUERYVISITOR_HPP
