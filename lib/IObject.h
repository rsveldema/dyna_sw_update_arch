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

} // namespace DUpdate

#endif