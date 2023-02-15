#include "Object.hpp"
#include "List.hpp"

namespace python::native
{
	/* Global Function */
	Object* len(List* args);

	/* class `string` */
	Object* string_upper(List* args);

	/* class `list` */
	Object* list_append(List* args);

}
