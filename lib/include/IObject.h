#ifndef IOBJECT__H___
#define IOBJECT__H___

#include <map>
#include <vector>
#include <assert.h>

#include "reflection.h"

namespace DUpdate
{

class UpdateableObject;

class IField
{
  public:
    const char* field_name;
    void* m_data = nullptr;

  public:
    IField(const char* field_name, UpdateableObject* parent);
};

/** The location of stuff in memory is fixed but their layout may change.
 */
class UpdateableObject
{
  public:
    UpdateableObject(const std::string& class_name)
    : clazz(getClassMetaData(class_name))
    {
        link(class_name);
    }

    void* getStorage()
    {
        assert(m_storage.size() > 0);
        return m_storage.data();
    }

    static Reflection::ClassMetaData& getClassMetaData(const std::string& class_name);
  
    void copy_linkage(UpdateableObject* parent);

    void registerField(IField* field);

  private:
    Reflection::ClassMetaData& clazz;
    std::vector<u_char> m_storage;
    std::vector<IField*> fields;

    void link(const std::string& class_name);
};

template <typename T> class UpdatedObject : public UpdateableObject
{
  public:
    UpdatedObject(const std::string& class_name) 
      : UpdateableObject(class_name)
    {
    }

    T& cast()
    {
      tmp.copy_linkage(this);
      return tmp;
    }

  private:
    T tmp;
};

/** a member of a class.
 * Members may be added/removed to create new versions of a struct on the fly.
 */
template <typename T> class Field : public IField
{
  public:
    Field(const char* field_name, UpdateableObject* parent) : IField(field_name, parent)
    {
    }

    Field(const Field& member) = delete;

    size_t getSize() const
    {
        return sizeof(T);
    }

    T& operator*()
    {
        return get();
    }
    const T& operator*() const
    {
        return get();
    }

    T& operator->()
    {
        return get();
    }

    const T& operator->() const
    {
        return get();
    }

    bool operator==(const Field& other) const
    {
        return get() == other.get();
    }

    bool operator!=(const Field& other) const
    {
        return get() != other.get();
    }

    bool operator>(const Field& other) const
    {
        return get() > other.get();
    }

    bool operator<(const Field& other) const
    {
        return get() < other.get();
    }

    bool operator<=(const Field& other) const
    {
        return get() <= other.get();
    }

    bool operator>=(const Field& other) const
    {
        return get() >= other.get();
    }

    T& operator=(const T& value)
    {
        get() = value;
        return get();
    }

    const T& get() const
    {
        assert(m_data);
        return *(T*)m_data;
    }

    T& get()
    {
        assert(m_data);
        return *(T*)m_data;
    }
};

#define FIELD(TYPE, NAME)                                                                                              \
    Field<TYPE> NAME                                                                                                   \
    {                                                                                                                  \
#NAME, this                                                                                                    \
    }


} // namespace DUpdate

#endif