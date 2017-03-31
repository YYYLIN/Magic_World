#ifndef _MAGICMATH_H_
#define _MAGICMATH_H_

/*
Include File:MagicMath.hpp
IOFO:graphics mathematics library
author:YYYLIN
Create time:2017 04 01
*/


namespace Magic
{
	template<typename T>    struct tvector4;
	template<typename T> 	struct tMatrix4x4;

	typedef tvector4<float>        vector4;
	typedef tMatrix4x4<float>      Matrix4;

	template<typename T>
	struct tvector4
	{
		//explicit 强制使用显示 转换
		explicit tvector4(value_type& const x, value_type& const y, value_type& const z, value_type& const w);

		typedef T value_type;
		union
		{
			struct { value_type  x, y, z, w; };
			struct { value_type  r, g, b, a; };
			struct { value_type  s, t, p, q; };
		}
	};

	template<typename T>
	struct tMatrix4x4
	{
		typedef T value_type;
		union
		{
			struct
			{
				value_type x1, x2, x3, x4;
				value_type y1, y2, y3, y4;
				value_type z1, z2, z3, z4;
				value_type w1, w2, w3, w4;
			};
			struct
			{
				tvector4<value_type> value[4];
			};
		};
	};


	template<typename T>
	tvector4<T>::tvector4(value_type& const x, value_type& const y, value_type& const z, value_type& const w)
	{

	}
};

#endif