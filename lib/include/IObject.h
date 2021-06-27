#ifndef IOBJECT__H___
#define IOBJECT__H___

#include <map>
#include <vector>

namespace DUpdate
{

/** The location of stuff in memory is fixed but their layout may change.
 */
class UpdateableObject
{
  public:
    UpdateableObject(const std::string& class_name)
    {
        auto& meta_data = getClassMetaData(class_name);
        if (!meta_data.linked)
        {
            meta_data.linked = true;
            
            link(class_name);
        }

        m_storage.resize(meta_data.getSize());
    }

    void* getStorage()
    {
        assert(m_storage.size() > 0);
        return m_storage.data();
    }

    static Reflection::ClassMetaData& getClassMetaData(const std::string& class_name);

  private:
    std::vector<u_char> m_storage;

    void link(const std::string& class_name);
};

template <typename T> class UpdatedObject : public UpdateableObject
{
  public:
    UpdatedObject(const std::string& class_name) : UpdateableObject(class_name)
    {
    }
};

/** a member of a class.
 * Members may be added/removed to create new versions of a struct on the fly.
 */
template <typename T> class Field
{
  public:
    Field(const char* field_name) : field_name(field_name)
    {
    }

    Field(const Field& member) = delete;

    size_t getSize() const
    {
        return sizeof(T);
    }

    T& operator*()
    {
        assert(m_data);
        return *m_data;
    }
    const T& operator*() const
    {
        assert(m_data);
        return *m_data;
    }

    T& operator->()
    {
        assert(m_data);
        return *m_data;
    }

    const T& operator->() const
    {
        assert(m_data);
        return *m_data;
    }

    bool operator==(const Field& other) const
    {
        assert(m_data);
        return *m_data == *other.m_data;
    }

    bool operator!=(const Field& other) const
    {
        assert(m_data);
        return *m_data != *other.m_data;
    }

    bool operator>(const Field& other) const
    {
        assert(m_data);
        return *m_data > *other.m_data;
    }

    bool operator<(const Field& other) const
    {
        assert(m_data);
        return *m_data < *other.m_data;
    }

    bool operator<=(const Field& other) const
    {
        assert(m_data);
        return *m_data <= *other.m_data;
    }

    bool operator>=(const Field& other) const
    {
        assert(m_data);
        return *m_data >= *other.m_data;
    }

    T& operator=(const T& value) const
    {
        assert(m_data);
        *m_data = value;
        return *m_data;
    }

  private:
    uint32_t m_offset = 0xffff'ffff;
    T* m_data = nullptr;
    const char* field_name;
};

#define FIELD(TYPE, NAME) Field<TYPE> NAME{#NAME}

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
    FIELD(int, d3);
    FIELD(int, d2);
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