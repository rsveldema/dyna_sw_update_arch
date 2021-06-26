#include "dyn_update.h"

namespace DUpdate
{
namespace
{
std::map<std::string, ClassMetaData> clazz_map;
}

const ClassMetaData& UpdateableObject::getClassMetaData(const std::string& class_name)
{
    auto it = clazz_map.find(class_name);
    if (it != clazz_map.end())
    {
        return it->second;
    }

    fprintf(stderr, "failed to find class %s in meta-data\n", class_name.c_str());
    abort();
}

} // namespace DUpdate