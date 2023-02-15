#pragma once

#include "Singleton.hpp"
#include "Object.hpp"
#include "List.hpp"

namespace python
{

    struct StringTable : Singleton<StringTable>
    {
        String* next_str;
        String* mod_str;
        String* init_str;
        String* add_str;
        String* len_str;
        String* call_str;
        String* getitem_str;
        String* setitem_str;
        String* getattr_str;
        String* setattr_str;
        String* name_str;
        String* iter_str;
        String* repr_str;

        String* libdir_pre_str;
        String* empty_str;
        String* so_pre_str;
        String* so_suf_str;
        String* pyc_suf_str;

        StringTable();

    };
      

} 

