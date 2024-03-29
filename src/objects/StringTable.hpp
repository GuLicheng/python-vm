#pragma once

#include "Singleton.hpp"
#include "Object.hpp"
#include "List.hpp"
#include "String.hpp"

namespace python
{
    #define STR(x) #x
    #define DefineString(x) inline static String* x = new String(STR(__##x##__))

    struct StringTable
    {
        // Magic methods
        DefineString(init);
        
        DefineString(getattr);
        DefineString(setattr);
        DefineString(getitem);
        DefineString(setitem);

        DefineString(module);
        DefineString(call);
        DefineString(name);
     
        // some operators
        DefineString(add);
        DefineString(sub);
        DefineString(mul);

        DefineString(len);
        DefineString(hash);
        DefineString(iter);
        DefineString(next);

        DefineString(le);
        DefineString(lt);
        DefineString(gt);
        DefineString(ge);
        DefineString(eq);
        DefineString(ne);
    };

    #undef STR
    #undef DefineString

    // struct StringTable1 : Singleton<StringTable>
    // {
    //     String* next_str;
    //     String* mod_str;
    //     String* init_str;
    //     String* add_str;
    //     String* len_str;
    //     String* call_str;
    //     String* getitem_str;
    //     String* setitem_str;
    //     String* getattr_str;
    //     String* setattr_str;
    //     String* name_str;
    //     String* iter_str;
    //     String* repr_str;

    //     String* libdir_pre_str;
    //     String* empty_str;
    //     String* so_pre_str;
    //     String* so_suf_str;
    //     String* pyc_suf_str;

    //     // StringTable();

    // };
      

} 

