
#include "basically.h"


namespace basically::runtime::variables
{


    Info::Info(std::string const& new_name,
               std::string const& new_type_name,
               size_t new_array_count,
               bool new_is_const)
    : name(new_name),
      type(new_type_name),
      array_count(new_array_count),
      is_const(new_is_const)
    {
    }


    Info::Info(ast::VariableDeclarationStatementPtr const& declaration)
    : name(declaration->name.text),
      type(declaration->type_name),
      initializer(declaration->initializer),
      array_count(0),
      is_const(false),
      visibility(typing::Visibility::Default)
    {
    }


    bool Info::is_array() const noexcept
    {
        return array_count != 0;
    }


    size_t Info::size() const noexcept
    {
        assert(type.resolved_type);

        auto computed_size = type.resolved_type->size();

        if (is_array())
        {
            return array_count * computed_size;
        }

        return computed_size;
    }


    Scope::Scope(ScopePtr& parent)
    : parent(parent)
    {
    }


    InfoPtr Scope::find(std::string const& name) const noexcept
    {
        auto found = variables.find(name);

        if (found != variables.end())
        {
            return found->second;
        }

        if (parent)
        {
            return parent->find(name);
        }

        return nullptr;
    }


    void Scope::insert(InfoPtr const& variable)
    {
        assert(variable);

        if (variables.find(variable->name) != variables.end())
        {
            throw std::runtime_error("Duplicate definition for " + variable->name);
        }

        variables.insert({ variable->name, variable });
    }


}
