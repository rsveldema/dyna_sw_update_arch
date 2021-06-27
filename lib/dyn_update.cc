#include "dyn_update.h"

namespace Reflection
{
namespace
{
std::map<std::string, Reflection::ClassMetaData*>* clazz_map;
}
std::map<std::string, Reflection::ClassMetaData*>& getClassMap()
{
    if (!clazz_map)
    {
        clazz_map = new std::map<std::string, Reflection::ClassMetaData*>();
    }
    return *clazz_map;
}

Reflection::ClassMetaData& __getClassMetaData(const std::string& class_name)
{
    auto& map = getClassMap();

    auto it = map.find(class_name);
    if (it != map.end())
    {
        return *it->second;
    }

    fprintf(stderr, "failed to find class %s in meta-data\n", class_name.c_str());
    abort();
}

    
} // namespace Reflection


namespace DUpdate
{
IField::IField(const char* field_name, UpdateableObject* parent)
{
    this->field_name = field_name;
    parent->registerField(this);
}

void UpdateableObject::registerField(IField* field)
{
    if (auto metaField = clazz.getFieldMetaData(field->field_name))
    {
        auto offset = metaField->offset;
        field->m_data = &m_storage[offset];

        fields.push_back(field);
    }
}

Reflection::ClassMetaData& UpdateableObject::getClassMetaData(const std::string& class_name)
{
    return Reflection::__getClassMetaData(class_name);
}

void UpdateableObject::link(const std::string& class_name)
{
    auto& clazz = getClassMetaData(class_name);
    clazz.link();

    m_storage.resize(clazz.size);
}

void UpdateableObject::copy_linkage(UpdateableObject* parent)
{
    for (auto f : fields)
    {
        if (auto metaField = parent->clazz.getFieldMetaData(f->field_name))
        {
            auto offset = metaField->offset;
            f->m_data = &parent->m_storage[offset];
        }
    }
}

} // namespace DUpdate
