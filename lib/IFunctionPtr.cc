#include "dyn_update.h"

namespace DUpdate
{
    namespace 
    {   
        struct FunctionInfo 
        {
            IFunctionPtr::ifunc_t func;
            std::string prototype;
        };


        std::map<std::string, size_t> func_map;
        std::vector<FunctionInfo> func_table;
    }

    IFunctionPtr::ifunc_t IFunctionPtr::get()
    {
        return func_table[func_index].func;
    }

    size_t IFunctionPtr::register_func_ptr(const std::string& name, ifunc_t func, const std::string& prototype)
    {
        auto it = func_map.find(name);
        if (it == func_map.end())
        {
            size_t new_ix = func_table.size();
            func_table.push_back({func, prototype});
            func_map[name] = new_ix;
            return new_ix;
        }

        return it->second;
    }

    void IFunctionPtr::do_replace(const std::string& name, ifunc_t func, const std::string& prototype)
    {
        auto it = func_map.find(name);
        if (it == func_map.end())
        {
            fprintf(stderr, "Failed to find function ptr: %s\n", name.c_str());
            return;
        }


        auto ix = it->second;
        if (func_table[ix].prototype != prototype)
        {
            fprintf(stderr, "Prototype compare failed: %s vs %s\n", func_table[ix].prototype.c_str(), prototype.c_str());
            abort();
        }
        func_table[ix].func = func;
    }

} // namespace DUpdate
