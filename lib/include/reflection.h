#ifndef REFLECTION_H_____
#define REFLECTION_H_____

#include <assert.h>

namespace Reflection
{
struct FieldMetaData
{
    std::string name;
    size_t getOffset();
};

struct ClassMetaData
{
    std::string name;
    std::string compat;
    std::vector<FieldMetaData> fields;

    bool linked = false;
    size_t size;

    ClassMetaData(const std::string& name, const std::string& compat, const std::vector<FieldMetaData>& fields)
    : name(name), compat(compat), fields(fields)
    {
        register_class();
    }

    size_t getSize() const
    {
        assert(linked);
        return size;
    }
    FieldMetaData& getFieldMetaData(const std::string& name);

    void register_class();
};
} // namespace Reflection

#endif