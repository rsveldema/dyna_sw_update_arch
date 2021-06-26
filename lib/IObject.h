#ifndef IOBJECT__H___
#define IOBJECT__H___

#include <map>

namespace DUpdate
{

/** The location of stuff in memory is fixed but their layout may change.
 */
class IObject
{
  public:
    std::map<std::string, int> mapping;
};


/** a member of a class.
 * Members may be added/removed to create new versions of a struct on the fly.
 */
class IMember
{
};

} // namespace DUpdate

#endif