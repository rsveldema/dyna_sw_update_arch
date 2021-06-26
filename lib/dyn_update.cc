#include "dyn_update.h"

namespace DUpdate
{
namespace
{
std::map<std::string, ClassMetaData> clazz_map;
}

const ClassMetaData& UpdateableObject::getClassMetaData(const std::string& class_name)
{
    return clazz_map[class_name];
}

} // namespace DUpdate