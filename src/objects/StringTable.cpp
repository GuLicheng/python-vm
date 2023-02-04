#include "StringTable.hpp"
#include "String.hpp"

namespace python
{
    StringTable::StringTable()
    {
        this->name_list = new List();

        for (auto str : names)
            name_list->append(new String(str));

    }
}
