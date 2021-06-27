#ifndef IFUNCTION_H_____
#define IFUNCTION_H_____

#include <map>

namespace DUpdate
{

class IFunctionPtr
{
  public:
    std::string name;
    typedef void (*ifunc_t)();
    size_t func_index = 0xffff'ffff;

  public:
    IFunctionPtr(const std::string& name, ifunc_t func, const std::string& prototype) : name(name)
    {
        func_index = register_func_ptr(name, func, prototype);
    }

    ifunc_t get();
    static size_t register_func_ptr(const std::string& name, ifunc_t func, const std::string& prototype);

    template <typename FUNC> static void replace(const std::string& name, FUNC func, const std::string& proto)
    {
        do_replace(name, (ifunc_t)func, proto);
    }

    template <typename FUNC> static void replace(const std::string& name, FUNC func)
    {
        do_replace(name, (ifunc_t)func, typeid(FUNC).name());
    }

    static void do_replace(const std::string& name, ifunc_t func, const std::string& prototype);
};

/** Functions are relocatable.
 */

template <class ...ArgTypes> 
class FunctionPtr : public IFunctionPtr
{
  public:
    FunctionPtr(const std::string& name, void(*func)(ArgTypes...)) : IFunctionPtr(name, (ifunc_t)func, typeid(func).name())
    {
    }

    void operator()(ArgTypes... args)
    {
        return ((void(*)(ArgTypes...))get())(args...);
    }
};

} // namespace DUpdate

#endif