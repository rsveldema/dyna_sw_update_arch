#ifndef REFLECTION_H_____
#define REFLECTION_H_____

#include <assert.h>
#include <map>

namespace Reflection
{
struct FieldMetaData
{
    std::string name;
    size_t size;
    size_t offset;

    size_t getSize() const
    {
        return size;
    }
    size_t getOffset() const
    {
        return offset;
    }
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
        registerClass();
    }

    size_t getSize() const
    {
        assert(linked);
        return size;
    }
    
    FieldMetaData* getFieldMetaData(const std::string& name)
    {
        for (auto& f : fields)
        {
            if (f.name == name)
            {
                return &f;
            }
        }
        fprintf(stderr, "failed to find field %s in class %s\n", name.c_str(), this->name.c_str());
        return nullptr;
    }

    void registerClass();

    void link();
};


ClassMetaData& getClassMetaData(const std::string& class_name);
std::map<std::string, Reflection::ClassMetaData*>& getClassMap();

} // namespace Reflection

#endif