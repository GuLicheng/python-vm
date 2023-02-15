#include "StringTable.hpp"
#include "String.hpp"

namespace python
{
    StringTable::StringTable()
    {
        this->next_str = new String("next");
        this->mod_str  = new String("__module__");
        this->init_str = new String("__init__");
        this->add_str  = new String("__add__");
        this->len_str  = new String("__len__");
        this->call_str = new String("__call__");
        this->name_str = new String("__name__");
        this->iter_str = new String("__iter__");
        this->repr_str = new String("__repr__");
        this->getitem_str = new String("__getitem__");
        this->setitem_str = new String("__setitem__");
        this->getattr_str = new String("__getattr__");
        this->setattr_str = new String("__setattr__");

        this->so_pre_str  = new String("lib");
        this->libdir_pre_str  = new String("./lib/");
        this->empty_str   = new String("");
        this->so_suf_str  = new String(".so");
        this->pyc_suf_str = new String(".pyc");
    }
}
