#include "dyn_update.h"


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
    else
    {
        fprintf(stderr, "registerField failed: something weird happened: failed to find field meta data (for %s)\n", field->field_name);
    }
}

Reflection::ClassMetaData& UpdateableObject::getClassMetaData(const std::string& class_name)
{
    return Reflection::getClassMetaData(class_name);
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
