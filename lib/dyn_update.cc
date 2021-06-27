#include "dyn_update.h"

namespace DUpdate
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

Reflection::ClassMetaData& UpdateableObject::getClassMetaData(const std::string& class_name)
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

void UpdateableObject::link(const std::string& class_name)
{
    auto& clazz = getClassMetaData(class_name);
    
}

} // namespace DUpdate

namespace Reflection
{
void ClassMetaData::register_class()
{
    auto& map = DUpdate::getClassMap();
    map[name] = this;
}
} // namespace Reflection
