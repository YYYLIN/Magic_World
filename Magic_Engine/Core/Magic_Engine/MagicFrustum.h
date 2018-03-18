#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_

#include <glm.hpp>

namespace Magic
{

	float WWTPlaneDotCoord(glm::vec4*, glm::vec3*);
	void WWTPlaneNormalize(glm::vec4*);

	class Frustum
	{
	public:
		Frustum();
		Frustum(const Frustum&);
		virtual ~Frustum();

		void ConstructFrustum(glm::mat4, glm::mat4); //构建平椎体

		bool CheckPoint(float, float, float); //检查点
		bool CheckCube(float, float, float, float);//检查立方体
		bool CheckSphere(float, float, float, float);//检查球
		bool CheckRectangle(float, float, float, float, float, float);//检查矩形
		//bool CheckPlaneLine(glm::vec2&, glm::vec2&, glm::vec3&);//检查平面直线

		//void GetMapArea(glm::vec2*, glm::vec2*);

		//	glm::vec4 m_planes[6];  //平面
		float frustum[6][4];
		//glm::vec2 Max, Min;
	};
};

#endif