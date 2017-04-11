#include <stdio.h>
#include "MagicMath.hpp"

int main()
{
	Magic::vector4 Vec4_1(1, 2, 3, 4);
	Magic::vector4 Vec4_2(0, 4, 5, 6);

	Magic::vector4 Vec4_3 = 3.0f * Vec4_1;

	return true;
}