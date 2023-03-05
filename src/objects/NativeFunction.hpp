#include "Object.hpp"
#include "List.hpp"

namespace python::native
{
	/* Global Function */
	Object* len(List* args);
	Object* isinstance(List* args);
	Object* type(List* args);
	Object* hash(List* args);
	Object* iter(List* args);
	Object* next(List* args);

	/* class `string` */
	Object* string_upper(List* args);

	/* class `list` */
	Object* list_append(List* args);
	Object* list_pop(List* args);
	Object* list_remove(List* args);
	// Object* list_reverse(List* args);
	// Object* list_sort(List* args);
	// Object* list_extend(List* args);
	// Object* list_index(List* args);
	// Object* list_getitem(List* args);

}
