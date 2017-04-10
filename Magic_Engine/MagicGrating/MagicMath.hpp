#ifndef _MAGICMATH_H_
#define _MAGICMATH_H_

/*
Include File:MagicMath.hpp
IOFO:graphics mathematics library
author:YYYLIN
Create time:2017 04 01
*/

#include <wchar.h>

namespace Magic
{
	template<typename T> 	struct tvector3;
	template<typename T>    struct tvector4;
	template<typename T> 	struct tMatrix4x4;


	template<typename T>
	struct tvector3
	{
		typedef T value_type;

		union
		{
			struct { value_type  x, y, z; };
			struct { value_type  r, g, b; };
			struct { value_type  s, t, p; };
		};

		tvector3();
		tvector3(tvector3<T> const & _tvector3);
		explicit tvector3(value_type const & _x);
		explicit tvector3(value_type const & _x, value_type const & _y, value_type const & _z);

		tvector3<T>& operator= (tvector3<T>  const & v);
	};

	template<typename T>
	struct tvector4
	{
		typedef T value_type;

		union
		{
			struct { value_type  x, y, z, w; };
			struct { value_type  r, g, b, a; };
			struct { value_type  s, t, p, q; };
		};

		//explicit 强制使用显示 转换
		tvector4();
		tvector4(tvector4<T> const & _tvector4);
		explicit tvector4(value_type const & _x);
		explicit tvector4(value_type const & _x, value_type const & _y, value_type const & _z, value_type const & _w);

		tvector4<T>& operator= (tvector4<T> const & v);
		tvector4<T>& operator*=(tvector4<T> const & v);
	};

	template<typename T>
	tvector4<T> operator* (tvector4<T> const & v1, tvector4<T> const & v2);

	/*
	行矩阵
	*/

	template<typename T>
	struct tMatrix4x4
	{
		typedef T value_type;

		union
		{
			struct
			{
				value_type x0, x1, x2, x3;
				value_type y0, y1, y2, y3;
				value_type z0, z1, z2, z3;
				value_type w0, w1, w2, w3;
			};
			struct
			{
				tvector4<T> value[4];
			};
		};

		tMatrix4x4();
		tMatrix4x4(tMatrix4x4<T> const & _tMatrix4x4);
		explicit tMatrix4x4(
			value_type const & _x0, value_type const & _x1, value_type const & _x2, value_type const & _x3,
			value_type const & _y0, value_type const & _y1, value_type const & _y2, value_type const & _y3,
			value_type const & _z0, value_type const & _z1, value_type const & _z2, value_type const & _z3,
			value_type const & _w0, value_type const & _w1, value_type const & _w2, value_type const & _w3);
		explicit tMatrix4x4(tvector4<T> const & _value0, tvector4<T> const & _value1, tvector4<T> const & _value2, tvector4<T> const & _value3);

		tMatrix4x4<T> & operator=  (tMatrix4x4<T> const & m);
		tMatrix4x4<T> & operator*= (tMatrix4x4<T> const & m);
	};

	template<typename T>
	tMatrix4x4<T> & operator* (tMatrix4x4<T> const & m1, tMatrix4x4<T> const & m2);



	typedef tvector3<float>        vector3;
	typedef tvector4<float>        vector4;
	typedef tMatrix4x4<float>      Matrix4;
}

#include "MagicMath.inl"

#endif