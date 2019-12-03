#pragma once

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <glm.hpp>

#pragma warning(push)
#pragma warning(disable:4251)

class MagicCamera
{
public:
	MagicCamera();
	~MagicCamera();

public:
	//一个计算取景变换的函数
	glm::mat4 CalculateViewMatrix();    //计算取景变换矩阵

										//三个Get系列函数
	glm::mat4 GetProjMatrix() { return m_matProj; }  //返回当前投影矩阵
	void GetCameraPosition(glm::vec3 *pVector) { *pVector = m_vCameraPosition; } //返回当前摄像机位置矩阵
	void GetTargetPosition(glm::vec3 *pLookat) { *pLookat = m_vTargetPosition; }
	void GetLookVector(glm::vec3 *pVector) { *pVector = m_vLookVector; }  //返回当前的观察矩阵
	void GetUpVector(glm::vec3 *pVectpr) { *pVectpr = m_vUpVector; }
	void GetRightVector(glm::vec3 *pVectpr) { *pVectpr = m_vRightVector; }

	//四个Set系列函数，注意他们都参数都有默认值NULL的，调用时不写参数也可以
	void SetTargetPosition(glm::vec3 *pLookat = NULL);  //设置摄像机的目标观察位置向量
	void SetCameraPosition(glm::vec3 *pVector = NULL); //设置摄像机所在的位置向量
	void SetViewMatrix(glm::mat4 *pMatrix = NULL);  //设置取景变换矩阵
	void SetProjMatrix(glm::mat4 *pMatrix = NULL);  //设置投影变换矩阵

public:
	void SetPosition(float _x, float _y, float _z);

	// 沿各分量平移的三个函数
	void MoveAlongRightVec(float fUnits);   // 沿right向量移动
	void MoveAlongUpVec(float fUnits);      // 沿up向量移动
	void MoveAlongLookVec(float fUnits);    // 沿look向量移动

											// 绕各分量旋转的三个函数
	void RotationRightVec(float fAngle);    // 绕right向量选择
	void RotationUpVec(float fAngle);       // 绕up向量旋转
	void RotationLookVec(float fAngle);     // 绕look向量旋转

	void SetRotation(float _Right, float _Look, float _Up);

private:
	//成员变量的声明
	glm::vec3				m_vRightVector;        // 右分量向量
	glm::vec3				m_vUpVector;           // 上分量向量
	glm::vec3				m_vLookVector;         // 观察方向向量
	glm::vec3				m_vCameraPosition;        // 摄像机位置的向量
	glm::vec3				m_vTargetPosition;        //目标观察位置的向量
	glm::mat4				m_matView;          // 取景变换矩阵
	glm::mat4				m_matProj;          // 投影变换矩阵   

	glm::vec3                m_XYVector;

};

#pragma warning(pop)