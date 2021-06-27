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

Reflection::ClassMetaData& getClassMetaData(const std::string& class_name)
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

    
void ClassMetaData::registerClass()
{
    auto& map = getClassMap();
    map[name] = this;
}

void ClassMetaData::link()
{
    if (linked)
    {
        return;
    }
    linked = true;

    if (compat != "")
    {
        auto& compat_clazz = getClassMetaData(compat);
        compat_clazz.link();
    }

    int offset = 0;
    for (auto& field : fields)
    {
        fprintf(stderr, "ASSIGN OFFSET: %s --> %d\n", field.name.c_str(), offset);
        field.offset = offset;
        offset += field.getSize();
    }

    this->size = offset;
}

} // namespace Reflection
