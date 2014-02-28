#pragma once
#include <gpm/contract.h>

typedef PLUGIN_API struct VECTOR2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float array[2];
	};

} VECTOR2, *PVECTOR2;
