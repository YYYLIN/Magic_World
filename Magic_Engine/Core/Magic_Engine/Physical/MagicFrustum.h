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

		void ConstructFrustum(glm::mat4, glm::mat4); //����ƽ׵��

		bool CheckPoint(float, float, float); //����
		bool CheckCube(float, float, float, float);//���������
		bool CheckSphere(float, float, float, float);//�����
		bool CheckRectangle(float, float, float, float, float, float);//������
		//bool CheckPlaneLine(glm::vec2&, glm::vec2&, glm::vec3&);//���ƽ��ֱ��

		//void GetMapArea(glm::vec2*, glm::vec2*);

		//	glm::vec4 m_planes[6];  //ƽ��
		float frustum[6][4];
		//glm::vec2 Max, Min;
	};
};

#endif