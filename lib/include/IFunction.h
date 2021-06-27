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
    IFunctionPtr(const std::string& name, ifunc_t func) : name(name)
    {
      func_index = register_func_ptr(name, func);
    }
    
    ifunc_t get();
    static size_t register_func_ptr(const std::string& name, ifunc_t func);

    template<typename FUNC>
    static void replace(const std::string& name, FUNC func)
    {
      do_replace(name, (ifunc_t) func);
    }
    
    static void do_replace(const std::string& name, ifunc_t func);
};

/** Functions are relocatable.
 */

template <typename func_t> class FunctionPtr : public IFunctionPtr
{
  public:
    FunctionPtr(const std::string& name, func_t func) : IFunctionPtr(name, (ifunc_t)func)
    {
    }

    void operator()()
    {
        return ((func_t)get())();
    }

    void replace(const std::string& name, ifunc_t func)
    {
      return IFunctionPtr::replace(name, func);
    }
};

} // namespace DUpdate

#endif