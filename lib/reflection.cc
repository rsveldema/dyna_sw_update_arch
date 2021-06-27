#include "dyn_update.h"

namespace Reflection
{
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
        auto& compat_clazz = __getClassMetaData(compat);
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
