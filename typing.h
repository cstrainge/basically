
#pragma once


namespace basically::typing
{


    struct NumberInfo;
    using NumberInfoPtr = std::shared_ptr<NumberInfo>;

    struct StructureInfo;
    using StructureInfoPtr = std::shared_ptr<StructureInfo>;

    using TypeExtraInfo = std::variant<NumberInfoPtr, StructureInfoPtr>;

    struct TypeInfo;
    using TypeInfoPtr = std::shared_ptr<TypeInfo>;
    using TypeInfoMap = std::unordered_map<std::string, TypeInfoPtr>;


    enum class Visibility
    {
        Public,
        Protected,
        Private,

        Default
    };


    template <Visibility default_value>
    constexpr Visibility resovle(Visibility value) noexcept
    {
        if (value == Visibility::Default)
        {
            return default_value;
        }

        return value;
    }


    template <Visibility default_value>
    constexpr bool check_visibility(Visibility value, Visibility desired) noexcept
    {
        assert(desired != Visibility::Default);
        return resovle<default_value>(value) == desired;
    }


    struct TypeInfo
    {
        std::string name;
        TypeExtraInfo extra;
        Visibility visibility;

        TypeInfo(std::string const& name,
                 TypeExtraInfo extra,
                 Visibility visibility = Visibility::Default);

        size_t size() const noexcept;
    };


    struct ExtraInfoHandler
    {
        std::function<void(NumberInfoPtr const&)> number;
        std::function<void(StructureInfoPtr const&)> structure;

        inline void operator ()(NumberInfoPtr const& value) const
        {
            number(value);
        }

        inline void operator ()(StructureInfoPtr const& value) const
        {
            structure(value);
        }
    };


    enum class SignedFlag : bool
    {
        IsSigned = true,
        IsUnsigned = false
    };


    enum class FloatingPointFlag : bool
    {
        IsFloatingPoint = true,
        IsInteger = false
    };


    struct NumberInfo
    {
        SignedFlag is_signed;
        FloatingPointFlag is_floating_point;

        size_t size;

        NumberInfo(SignedFlag new_is_signed,
                   FloatingPointFlag new_is_floating_point,
                   size_t new_size) noexcept;
    };


    struct FieldInfo
    {
        std::string name;
        TypeInfoPtr type;
        size_t offset;
        ast::Expression initializer;
    };


    using FieldInfoList = std::vector<FieldInfo>;


    struct StructureInfo : public TypeInfo
    {
        FieldInfoList fields;
    };


    struct ParameterInfo
    {
        std::string name;
        TypeInfoPtr type;
        ast::Expression initializer;
    };


    using ParameterList = std::vector<ParameterInfo>;


    struct SubInfo
    {
        std::string name;
        ParameterList parameters;
        Visibility visibility = Visibility::Default;
        ast::StatementList body;
    };


    using SubInfoPtr = std::shared_ptr<SubInfo>;
    using SubInfoMap = std::unordered_map<std::string, SubInfoPtr>;


    struct FunctionInfo : public SubInfo
    {
        TypeInfoPtr return_type;
    };


    using FunctionInfoPtr = std::shared_ptr<FunctionInfo>;
    using FunctionInfoMap = std::unordered_map<std::string, FunctionInfoPtr>;


}
