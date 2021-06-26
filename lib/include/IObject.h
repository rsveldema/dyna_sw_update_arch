#ifndef IOBJECT__H___
#define IOBJECT__H___

#include <map>
#include <vector>

namespace DUpdate
{
class FieldMetaData
{
  public:
    uint16_t offset;
};

class ClassMetaData
{
  public:
    uint16_t size;
    mutable std::map<std::string, FieldMetaData> fields;

    const FieldMetaData& getFieldMetaData(const std::string& field_name) const
    {
        return fields[field_name];
    }
};



/** The location of stuff in memory is fixed but their layout may change.
 */
class UpdateableObject
{
  public:
    UpdateableObject(const std::string& class_name)
    {
        auto& meta_data = getClassMetaData(class_name);
        m_storage.resize(meta_data.size);
    }

    void *getStorage()
    {
        return m_storage.data();
    }

    static const ClassMetaData& getClassMetaData(const std::string& class_name);


private:
    std::vector<u_char> m_storage;
};

/** a member of a class.
 * Members may be added/removed to create new versions of a struct on the fly.
 */
template <typename T> class Member
{
  public:
    Member(UpdateableObject* parent, const T& value, const char* class_name, const char* field_name)
    {
        auto& clazz = UpdateableObject::getClassMetaData(class_name);
        auto& field = clazz.getFieldMetaData(field_name);
        auto offset = field.offset;
        m_data = (T*) parent->getStorage() + offset;
    }

  private:
    T* m_data = nullptr;
};

template <typename T> class UpdatedObject : public UpdateableObject
{
};

class MyData_V1 : public UpdateableObject
{
  public:
    Member<int> d1{this, 1234, typeid(*this).name(), "d1"};
    Member<int> d2{this, 5678, typeid(*this).name(), "d2"};
};

class MyData_V2 : public UpdatedObject<MyData_V1>
{
  public:
    Member<int> d1{this, 4321, typeid(*this).name(), "d1"};
    Member<int> d1a{this, 1111, typeid(*this).name(), "d1a"};
    Member<int> d2{this, 8765, typeid(*this).name(), "d2"};
};

} // namespace DUpdate

#endif