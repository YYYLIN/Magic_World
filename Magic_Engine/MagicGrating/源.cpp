#include <stdio.h>
#include "MagicMath.hpp"

int main()
{
	Magic::vector3 Vec3(1, 2, 3);

	Magic::vector3 VEC3_1(Vec3);

	Magic::vector3 VEC3_4(1.0f);

	Magic::vector3 VVV;

	Magic::vector3 VEC3_2 = VEC3_1;
	VEC3_1 = VEC3_4;

	return true;
}