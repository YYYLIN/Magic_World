#ifndef _MAGICMATH_INL_
#define _MAGICMATH_INL_

/*
Source File:MagicMath.inl
IOFO:graphics mathematics library
author:YYYLIN
Create time:2017 04 01
*/

namespace Magic
{

	//-----------------tvector3-------------------------------------------
	template<typename T>
	tvector3<T>::tvector3()
	{
		x = y = z = 0;
	}

	template<typename T>
	tvector3<T>::tvector3(tvector3<T> const & _tvector3) :
		x(_tvector3.x), y(_tvector3.y), z(_tvector3.z)
	{}

	template<typename T>
	tvector3<T>::tvector3(value_type const & _x) :
		x(_x), y(_x), z(_x)
	{}

	template<typename T>
	tvector3<T>::tvector3(value_type const & _x, value_type const & _y, value_type const & _z) :
		x(_x), y(_y), z(_z)
	{}

	template<typename T>
	tvector3<T>& tvector3<T>::operator= (tvector3<T> const & v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	//-----------------tvector3-------------------------------------------
	//-----------------tvector4-------------------------------------------
	template<typename T>
	tvector4<T>::tvector4()
	{
		//	x = y = z = w = 0;
		memset(this, 0x00, sizeof(tvector4<T>));
	}

	template<typename T>
	tvector4<T>::tvector4(tvector4<T> const & _tvector4) :
		x(_tvector4.x), y(_tvector4.y), z(_tvector4.z), w(_tvector4.w)
	{}

	template<typename T>
	tvector4<T>::tvector4(value_type const & _x) :
		x(_x), y(_x), z(_x), w(_x)
	{}

	template<typename T>
	tvector4<T>::tvector4(value_type const & _x, value_type const & _y, value_type const & _z, value_type const & _w) :
		x(_x), y(_y), z(_z), w(_w)
	{}

	template<typename T>
	tvector4<T>& tvector4<T>::operator= (tvector4<T> const & v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	template<typename T>
	tvector4<T>& tvector4<T>::operator*=(tvector4<T> const & v)
	{
		return (*this = *this * v);
	}

	template<typename T>
	tvector4<T> operator+ (tvector4<T> const & v1, tvector4<T> const & v2)
	{
		return tvector4<T>(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z,
			v1.w + v2.w
			);
	}

	template<typename T>
	tvector4<T> operator* (typename tvector4<T>::value_type const & s, tvector4<T> const & v)
	{
		return tvector4<T>(
			s * v.x,
			s * v.y,
			s * v.z,
			s * v.w
			);
	}

	template<typename T>
	tvector4<T> operator* (tvector4<T> const & v, typename tvector4<T>::value_type const & s)
	{
		return tvector4<T>(
			v.x * s,
			v.y * s,
			v.z * s,
			v.w * s
			);
	}

	template<typename T>
	tvector4<T> operator*(tvector4<T> const & v1, tvector4<T> const & v2)
	{
		return tvector4<T>(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z,
			v1.w * v2.w
			);
	}

	//-----------------tvector4-------------------------------------------

	//-----------------tMatrix4x4-------------------------------------------
	template<typename T>
	tMatrix4x4<T>::tMatrix4x4()
	{
		memset(this, 0x00, sizeof(tMatrix4x4<T>));
	}

	template<typename T>
	tMatrix4x4<T>::tMatrix4x4(tMatrix4x4<T> const & _tMatrix4x4)
	{
		/*
		value[0] = _tMatrix4x4.value[0];
		value[1] = _tMatrix4x4.value[1];
		value[2] = _tMatrix4x4.value[2];
		value[3] = _tMatrix4x4.value[3];*/
		memcpy(this, &_tMatrix4x4, sizeof(tMatrix4x4<T>));
	}

	template<typename T>
	tMatrix4x4<T>::tMatrix4x4(
		value_type const & _x0, value_type const & _x1, value_type const & _x2, value_type const & _x3,
		value_type const & _y0, value_type const & _y1, value_type const & _y2, value_type const & _y3,
		value_type const & _z0, value_type const & _z1, value_type const & _z2, value_type const & _z3,
		value_type const & _w0, value_type const & _w1, value_type const & _w2, value_type const & _w3) :
		x0(_x0), x1(_x1), x2(_x2), x3(_x3),
		y0(_y0), y1(_y1), y2(_y2), y3(_y3),
		z0(_z0), z1(_z1), z2(_z2), z3(_z3),
		w0(_w0), w1(_w1), w2(_w2), w3(_w3)
	{}

	template<typename T>
	tMatrix4x4<T>::tMatrix4x4(tvector4<T> const & _value0, tvector4<T> const & _value1, tvector4<T> const & _value2, tvector4<T> const & _value3)
	{
		value[0] = _value0;
		value[1] = _value1;
		value[2] = _value2;
		value[3] = _value3;
	}

	template<typename T>
	tMatrix4x4<T> & tMatrix4x4<T>::operator=(tMatrix4x4<T> const & m)
	{
		/*
		value[0] = m.value[0];
		value[1] = m.value[1];
		value[2] = m.value[2];
		value[3] = m.value[3];*/
		memcpy(this, &m, sizeof(tMatrix4x4<T>));
		return *this;
	}

	template<typename T>
	tMatrix4x4<T> & tMatrix4x4<T>::operator*= (tMatrix4x4<T> const & m)
	{
		return (*this = *this * m);
	}

	template<typename T>
	tMatrix4x4<T> operator* (tMatrix4x4<T> const & m1, tMatrix4x4<T> const & m2)
	{
		return tMatrix4x4<T>(
			m1.x0 * m2.x0 + m1.x1 * m2.y0 + m1.x2 * m2.z0 + m1.x3 * m2.w0,
			m1.x0 * m2.x1 + m1.x1 * m2.y1 + m1.x2 * m2.z1 + m1.x3 * m2.w1,
			m1.x0 * m2.x2 + m1.x1 * m2.y2 + m1.x2 * m2.z2 + m1.x3 * m2.w2,
			m1.x0 * m2.x3 + m1.x1 * m2.y3 + m1.x2 * m2.z3 + m1.x3 * m2.w3,

			m1.y0 * m2.x0 + m1.y1 * m2.y0 + m1.y2 * m2.z0 + m1.y3 * m2.w0,
			m1.y0 * m2.x1 + m1.y1 * m2.y1 + m1.y2 * m2.z1 + m1.y3 * m2.w1,
			m1.y0 * m2.x2 + m1.y1 * m2.y2 + m1.y2 * m2.z2 + m1.y3 * m2.w2,
			m1.y0 * m2.x3 + m1.y1 * m2.y3 + m1.y2 * m2.z3 + m1.y3 * m2.w3,

			m1.z0 * m2.x0 + m1.z1 * m2.y0 + m1.z2 * m2.z0 + m1.z3 * m2.w0,
			m1.z0 * m2.x1 + m1.z1 * m2.y1 + m1.z2 * m2.z1 + m1.z3 * m2.w1,
			m1.z0 * m2.x2 + m1.z1 * m2.y2 + m1.z2 * m2.z2 + m1.z3 * m2.w2,
			m1.z0 * m2.x3 + m1.z1 * m2.y3 + m1.z2 * m2.z3 + m1.z3 * m2.w3,

			m1.w0 * m2.x0 + m1.w1 * m2.y0 + m1.w2 * m2.z0 + m1.w3 * m2.w0,
			m1.w0 * m2.x1 + m1.w1 * m2.y1 + m1.w2 * m2.z1 + m1.w3 * m2.w1,
			m1.w0 * m2.x2 + m1.w1 * m2.y2 + m1.w2 * m2.z2 + m1.w3 * m2.w2,
			m1.w0 * m2.x3 + m1.w1 * m2.y3 + m1.w2 * m2.z3 + m1.w3 * m2.w3
			);
	}

	//-----------------tMatrix4x4-------------------------------------------

}

#endif
