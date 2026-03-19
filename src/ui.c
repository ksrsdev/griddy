#include "ui.h"

#include <stdbool.h>


bool Text_UITypeHasTextWrapped(UIType type)
{
	if (
			(type == UI_TYPE_TEXT_WRAPPED)
	   ) {
		return true;
	} else {
		return false;
	}
}
