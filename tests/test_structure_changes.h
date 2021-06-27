#ifndef TEST_DATA_STR_H____
#define TEST_DATA_STR_H____

#include <dyn_update.h>

namespace DUpdate
{

class MyData_V1 : public UpdateableObject
{
  public:
    MyData_V1() : UpdateableObject(typeid(*this).name())
    {
        d1 = 1234;
        d2 = 5678;
    }
    FIELD(int, d1);
    FIELD(int, d2);
};

class MyData_V2 : public UpdatedObject<MyData_V1>
{
  public:
    MyData_V2() : UpdatedObject<MyData_V1>(typeid(*this).name())
    {
        d1 = 1234;
        d3 = 1111;
        d2 = 5678;
    }
    FIELD(int, d1);
    FIELD(int, d2);
    FIELD(int, d3);
};

class MyData_V3 : public UpdatedObject<MyData_V1>
{
  public:
    MyData_V3() : UpdatedObject<MyData_V1>(typeid(*this).name())
    {
        d2 = 9999;
    }
    FIELD(int, d2);
};

} // namespace DUpdate

#endif