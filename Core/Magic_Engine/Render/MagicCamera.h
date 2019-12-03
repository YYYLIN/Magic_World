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
	//һ������ȡ���任�ĺ���
	glm::mat4 CalculateViewMatrix();    //����ȡ���任����

										//����Getϵ�к���
	glm::mat4 GetProjMatrix() { return m_matProj; }  //���ص�ǰͶӰ����
	void GetCameraPosition(glm::vec3 *pVector) { *pVector = m_vCameraPosition; } //���ص�ǰ�����λ�þ���
	void GetTargetPosition(glm::vec3 *pLookat) { *pLookat = m_vTargetPosition; }
	void GetLookVector(glm::vec3 *pVector) { *pVector = m_vLookVector; }  //���ص�ǰ�Ĺ۲����
	void GetUpVector(glm::vec3 *pVectpr) { *pVectpr = m_vUpVector; }
	void GetRightVector(glm::vec3 *pVectpr) { *pVectpr = m_vRightVector; }

	//�ĸ�Setϵ�к�����ע�����Ƕ���������Ĭ��ֵNULL�ģ�����ʱ��д����Ҳ����
	void SetTargetPosition(glm::vec3 *pLookat = NULL);  //�����������Ŀ��۲�λ������
	void SetCameraPosition(glm::vec3 *pVector = NULL); //������������ڵ�λ������
	void SetViewMatrix(glm::mat4 *pMatrix = NULL);  //����ȡ���任����
	void SetProjMatrix(glm::mat4 *pMatrix = NULL);  //����ͶӰ�任����

public:
	void SetPosition(float _x, float _y, float _z);

	// �ظ�����ƽ�Ƶ���������
	void MoveAlongRightVec(float fUnits);   // ��right�����ƶ�
	void MoveAlongUpVec(float fUnits);      // ��up�����ƶ�
	void MoveAlongLookVec(float fUnits);    // ��look�����ƶ�

											// �Ƹ�������ת����������
	void RotationRightVec(float fAngle);    // ��right����ѡ��
	void RotationUpVec(float fAngle);       // ��up������ת
	void RotationLookVec(float fAngle);     // ��look������ת

	void SetRotation(float _Right, float _Look, float _Up);

private:
	//��Ա����������
	glm::vec3				m_vRightVector;        // �ҷ�������
	glm::vec3				m_vUpVector;           // �Ϸ�������
	glm::vec3				m_vLookVector;         // �۲췽������
	glm::vec3				m_vCameraPosition;        // �����λ�õ�����
	glm::vec3				m_vTargetPosition;        //Ŀ��۲�λ�õ�����
	glm::mat4				m_matView;          // ȡ���任����
	glm::mat4				m_matProj;          // ͶӰ�任����   

	glm::vec3                m_XYVector;

};

#pragma warning(pop)