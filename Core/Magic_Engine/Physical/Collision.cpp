#include "Collision.h"

namespace Magic
{

	bool IsTrueBOOLVec3(glm::bvec3 _bool)
	{
		return _bool.x && _bool.y && _bool.z;
	}

	glm::bvec3 XMBOOLVec3OrInt(glm::bvec3 _bool1, glm::bvec3 _bool2)
	{
		return glm::bvec3((_bool1.x | _bool2.x) != 0, (_bool1.y | _bool2.y) != 0, (_bool1.z | _bool2.z) != 0);
	}

	bool XMVec3GreaterOrEqual(glm::vec3 _vec1, glm::vec3 _vec2)
	{
		return _vec1.x >= _vec2.x && _vec1.y >= _vec2.y && _vec1.z >= _vec2.z;
	}

	bool XMVec3LessOrEqual(glm::vec3 _vec1, glm::vec3 _vec2)
	{
		return _vec1.x <= _vec2.x && _vec1.y <= _vec2.y && _vec1.z <= _vec2.z;
	}

	bool XMVec3EqualInt(glm::bvec3 _bool1, glm::bvec3 _bool2)
	{
		return (_bool1.x == _bool2.x && _bool1.y == _bool2.y && _bool1.z == _bool2.z) != 0;
	}

	glm::vec3 XMVec3Dot(glm::vec3 _vec1, glm::vec3 _vec2)
	{
		return glm::vec3(glm::dot(_vec1, _vec2));
	}

	glm::vec3 XMVecMultiplayAdd(glm::vec3 _vec1, glm::vec3 _vec2, glm::vec3 _vec3)
	{
		return glm::vec3(
			_vec1.x * _vec2.x + _vec3.x,
			_vec1.y * _vec2.y + _vec3.y,
			_vec1.z * _vec2.z + _vec3.z);
	}

	/*
	功能：射线和三角形的相交检测（顺时针正面）
	参数：
	Origin = 射线坐标
	Direction = 射线方向
	V0 = 顶点0
	V1 = 顶点1
	V2 = 顶点2
	pDist = 返回的点
	返回值：
	bool = 成功true
	*/
	bool IntersectRayTriangle(glm::vec3 Origin, glm::vec3 Direction, glm::vec3 V0, glm::vec3 V1, glm::vec3 V2, float* pDist)
	{
		static const glm::vec3 Epsilon(1e-20f);

		glm::vec3 Zero;

		glm::vec3 e1 = V1 - V0;
		glm::vec3 e2 = V2 - V0;

		glm::vec3 p = glm::cross(Direction, e2);

		glm::vec3 det = XMVec3Dot(e1, p);

		glm::vec3 u, v, t;
		glm::bvec3 Vec3BOOL;

		if (XMVec3GreaterOrEqual(det, Epsilon))
		{
			glm::vec3 s = Origin - V0;

			u = XMVec3Dot(s, p);
			glm::bvec3 NoIntersection = glm::lessThan(u, Zero);
			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::greaterThan(u, det));

			glm::vec3 q = glm::cross(s, e1);

			v = XMVec3Dot(Direction, q);

			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::lessThan(v, Zero));
			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::greaterThan(u + v, det));

			t = XMVec3Dot(e2, q);

			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::lessThan(t, Zero));

			if (XMVec3EqualInt(NoIntersection, glm::bvec3(true)))
				return false;
		}
		else if (XMVec3LessOrEqual(det, -Epsilon))
		{
			glm::vec3 s = Origin - V0;

			u = XMVec3Dot(s, p);

			glm::bvec3 NoIntersection = glm::greaterThan(u, Zero);
			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::lessThan(u, det));

			glm::vec3 q = glm::cross(s, e1);

			v = XMVec3Dot(Direction, q);

			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::greaterThan(v, Zero));
			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::lessThan(u + v, det));

			t = XMVec3Dot(e2, q);

			NoIntersection = XMBOOLVec3OrInt(NoIntersection, glm::greaterThan(t, Zero));

			if (XMVec3EqualInt(NoIntersection, glm::bvec3(true)))
				return false;
		}
		else
		{
			return false;
		}
	
		glm::vec3 inv_det = 1.0f / det;

		t *= inv_det;

		*pDist = t.x;

		return true;
	}

}