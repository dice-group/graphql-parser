#ifndef GRAPHQL_PARSER_SCHEMAVISITOR_HPP
#define GRAPHQL_PARSER_SCHEMAVISITOR_HPP

#include <GraphQL/GraphQLBaseVisitor.h>

#include "Dice/graphql-parser/internal/TypeDefinitions.hpp"

namespace Dice::graphql_parser::visitor {

	using namespace Dice::graphql_parser::internal;

    class SchemaVisitor : public Dice::graphql_parser::base::GraphQLBaseVisitor {

	private:

		std::shared_ptr<TypeDefinitions> type_definitions = std::make_shared<TypeDefinitions>();

    public:

		const std::shared_ptr<TypeDefinitions> &getTypeDefinitions() {
			return type_definitions;
		}

		antlrcpp::Any visitDocument(base::GraphQLParser::DocumentContext *ctx) override {
            for(const auto &definition : ctx->definition())
				if(definition->typeSystemDefinition())
					visitTypeSystemDefinition(definition->typeSystemDefinition());
            return nullptr;
		}

		antlrcpp::Any visitTypeSystemDefinition(base::GraphQLParser::TypeSystemDefinitionContext *ctx) override {
			if(ctx->typeDefinition())
				visitTypeDefinition(ctx->typeDefinition());
            return nullptr;
		}

		antlrcpp::Any visitTypeDefinition(base::GraphQLParser::TypeDefinitionContext *ctx) override {
			if(ctx->objectTypeDefinition())
				type_definitions->object_definitions.emplace_back(
						std::move(visitObjectTypeDefinition(ctx->objectTypeDefinition()).as<std::unique_ptr<ObjectDefinition>>()));
			else if(ctx->interfaceTypeDefinition())
				type_definitions->interface_definitions.emplace_back(
                        std::move(visitInterfaceTypeDefinition(ctx->interfaceTypeDefinition()).as<std::unique_ptr<InterfaceDefinition>>()));
            return nullptr;
		}

		antlrcpp::Any visitObjectTypeDefinition(base::GraphQLParser::ObjectTypeDefinitionContext *ctx) override {
			// create new object definition
			auto obj_def = std::make_unique<ObjectDefinition>();
			// store the name of the type
            obj_def->name = ctx->name()->getText();
			// iterate over the directives of the object definition
			if(ctx->directives())
				for(const auto &dir_ctx : ctx->directives()->directive())
                    obj_def->directives.emplace_back(std::move(visitDirective(dir_ctx).as<std::unique_ptr<Directive>>()));
			// iterate over the field definitions of the object definition
			if(ctx->fieldsDefinition())
                for(const auto &fd_ctx : ctx->fieldsDefinition()->fieldDefinition())
                    obj_def->field_definitions.emplace_back(
							std::move(visitFieldDefinition(fd_ctx).as<std::unique_ptr<FieldDefinition>>()));
			// iterate over the implemented interfaces
            auto current_ctx = ctx->implementsInterfaces();
			while(current_ctx) {
				obj_def->implemented_interfaces.push_back(current_ctx->namedType()->name()->getText());
				current_ctx = (current_ctx->implementsInterfaces());
			}
            return obj_def;
		}

		antlrcpp::Any visitInterfaceTypeDefinition(base::GraphQLParser::InterfaceTypeDefinitionContext *ctx) override {
            // create new interface definition
            auto interface_def = std::make_unique<InterfaceDefinition>();
            // store the name of the interface
            interface_def->name = ctx->name()->getText();
            // iterate over the directives of the interface definition
            if(ctx->directives())
                for(const auto &dir_ctx : ctx->directives()->directive())
                    interface_def->directives.emplace_back(
							std::move(visitDirective(dir_ctx).as<std::unique_ptr<Directive>>()));
            // iterate over the field definitions of the interface definition
            if(ctx->fieldsDefinition())
                for(const auto &fd_ctx : ctx->fieldsDefinition()->fieldDefinition())
                    interface_def->field_definitions.emplace_back(
							std::move(visitFieldDefinition(fd_ctx).as<std::unique_ptr<FieldDefinition>>()));
            return interface_def;
		}

		antlrcpp::Any visitFieldDefinition(base::GraphQLParser::FieldDefinitionContext *ctx) override {
			// create new field definition
            auto field_def = std::make_unique<FieldDefinition>();
			// store the name of the field
			field_def->name = ctx->name()->getText();
			// iterate over the directives of the field definition
            if(ctx->directives())
                for(const auto &dir_ctx : ctx->directives()->directive())
                    field_def->directives.emplace_back(std::move(visitDirective(dir_ctx).as<std::unique_ptr<Directive>>()));
			// find the type of the field
			if(ctx->type_()) {
				// check if it is a non-null type
				if(ctx->type_()->getText().ends_with('!'))
					field_def->non_null = true;
				// check if it is a list type
				if(ctx->type_()->listType()) {
					field_def->is_list = true;
					// check if list is allowed toh have null values
					if(ctx->type_()->listType()->type_()->getText().ends_with('!'))
						field_def->null_list_values = false;
					// get the name of the type
					field_def->type_name = ctx->type_()->listType()->type_()->namedType()->getText();
				}
				// get the name of the type
				else
                    field_def->type_name = ctx->type_()->namedType()->getText();
			}
            return field_def;
		}

		antlrcpp::Any visitDirective(base::GraphQLParser::DirectiveContext *ctx) override {
            auto directive = std::make_unique<Directive>();
            directive->name = ctx->name()->getText();
            if(ctx->arguments()) {
                for(const auto &argument : ctx->arguments()->argument()) {
                    auto arg_name = argument->name()->getText();
                    auto value = visitValue(argument->value());
                    directive->arguments[arg_name] = value.as<std::string>();
                }
            }
			return directive;
		}

        antlrcpp::Any visitValue(base::GraphQLParser::ValueContext *ctx) override {
			if(ctx->objectValue())
                throw std::runtime_error("ObjectValues in arguments are not supported");
			if(ctx->listValue())
                throw std::runtime_error("Lists of values in arguments are not supported");
            if(ctx->stringValue())
				return ctx->stringValue()->getText();
			else if(ctx->intValue())
                return ctx->intValue()->getText();
            else if(ctx->floatValue())
                return ctx->floatValue()->getText();
            else if(ctx->booleanValue())
                return ctx->booleanValue()->getText();
			return nullptr;
        }

	};
}


#endif //GRAPHQL_PARSER_SCHEMAVISITOR_HPP
