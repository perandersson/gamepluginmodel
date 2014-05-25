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

inline void VECTOR2_Add(PVECTOR2 v, const PVECTOR2 rhs) {
	v->x += rhs->x;
	v->y += rhs->y;
}

inline void VECTOR2_Add(PVECTOR2 v, float x, float y) {
	v->x += x;
	v->y += y;
}

inline void VECTOR2_Sub(PVECTOR2 v, const PVECTOR2 rhs) {

}

inline void VECTOR2_Mul(PVECTOR2 v, const PVECTOR2 rhs) {

}
