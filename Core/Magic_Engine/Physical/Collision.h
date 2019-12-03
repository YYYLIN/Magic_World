#pragma once

#include "glm.hpp"

namespace Magic
{
	bool IsTrueBOOLVec3(glm::bvec3 _bool);
	glm::bvec3 XMBOOLVec3OrInt(glm::bvec3 _bool1, glm::bvec3 _bool2);
	bool XMVec3GreaterOrEqual(glm::vec3 _vec1, glm::vec3 _vec2);
	bool XMVec3LessOrEqual(glm::vec3 _vec1, glm::vec3 _vec2);
	bool XMVec3EqualInt(glm::bvec3 _bool1, glm::bvec3 _bool2);
	glm::vec3 XMVec3Dot(glm::vec3 _vec1, glm::vec3 _vec2);
	glm::vec3 XMVecMultiplayAdd(glm::vec3 _vec1, glm::vec3 _vec2, glm::vec3 _vec3);

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
	bool IntersectRayTriangle(glm::vec3 Origin, glm::vec3 Direction, glm::vec3 V0, glm::vec3 V1, glm::vec3 V2, float* pDist);
}