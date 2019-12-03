#include "MagicCamera.h"
#include "Math/MagicMath.h"


MagicCamera::MagicCamera()
{
	//	m_pd3dDevice = pd3dDevice;
	m_vRightVector = glm::vec3(1.0f, 0.0f, 0.0f);   // Ĭ����������X�������غ�
	m_vUpVector = glm::vec3(0.0f, 1.0f, 0.0f);   // Ĭ����������Y�������غ�
	m_vLookVector = glm::vec3(0.0f, 0.0f, 1.0f);   // Ĭ�Ϲ۲�������Z�������غ�
	m_vCameraPosition = glm::vec3(0.0f, 0.0f, -20.0f);   // Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)
	m_vTargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);//Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);
	m_XYVector = glm::vec3(1.0f, 0.0f, 0.0f);
}


MagicCamera::~MagicCamera()
{
}


//-----------------------------------------------------------------------------
// Name��CameraClass::CalculateViewMatrix( )
// Desc: ���ݸ����ľ�������ȡ���任����
//-----------------------------------------------------------------------------
glm::mat4 MagicCamera::CalculateViewMatrix()
{
	glm::mat4 pMatrix;
	//1.�Ȱ�3���������淶����ʹ���໥��ֱ����Ϊһ����������
	m_vLookVector = glm::normalize(m_vLookVector);//�淶���۲����
	m_vUpVector = glm::cross(m_vLookVector, m_vRightVector); // ��������۲�������ֱ
	m_vUpVector = glm::normalize(m_vUpVector); // �淶��������              
	m_vRightVector = glm::cross(m_vUpVector, m_vLookVector);	// ����������������ֱ
	m_vRightVector = glm::normalize(m_vRightVector); // �淶��������


													 // 2.������ȡ���任����
													 //����д��ȡ���任����ĵ�һ��
	pMatrix[0].x = m_vRightVector.x;           // Rx
	pMatrix[0].y = m_vUpVector.x;              // Ux
	pMatrix[0].z = m_vLookVector.x;            // Lx
	pMatrix[0].w = 0.0f;
	//����д��ȡ���任����ĵڶ���
	pMatrix[1].x = m_vRightVector.y;           // Ry
	pMatrix[1].y = m_vUpVector.y;              // Uy
	pMatrix[1].z = m_vLookVector.y;            // Ly
	pMatrix[1].w = 0.0f;
	//����д��ȡ���任����ĵ�����
	pMatrix[2].x = m_vRightVector.z;           // Rz
	pMatrix[2].y = m_vUpVector.z;              // Uz
	pMatrix[2].z = m_vLookVector.z;            // Lz
	pMatrix[2].w = 0.0f;
	//����д��ȡ���任����ĵ�����

	pMatrix[3].x = -glm::dot(m_vRightVector, m_vCameraPosition);         // -P*R
	pMatrix[3].y = -glm::dot(m_vUpVector, m_vCameraPosition);       // -P*U
	pMatrix[3].z = -glm::dot(m_vLookVector, m_vCameraPosition);     // -P*L
	pMatrix[3].w = 1.0f;

	return pMatrix;
}


//-----------------------------------------------------------------------------
// Name��CameraClass::SetTargetPosition( )
// Desc: ����������Ĺ۲�λ��
//-----------------------------------------------------------------------------
void MagicCamera::SetTargetPosition(glm::vec3 *pLookat)
{
	//�ȿ���pLookat�Ƿ�ΪĬ��ֵNULL
	if (pLookat != NULL)
		m_vTargetPosition = (*pLookat);
	else
		m_vTargetPosition = glm::vec3(0.0f, 0.0f, 1.0f);

	m_vLookVector = m_vTargetPosition - m_vCameraPosition;//�۲��λ�ü������λ�ã��õ��۲췽������
	m_vLookVector = glm::normalize(m_vLookVector);//�淶��m_vLookVector����

												  //�������淶��m_vUpVector��m_vRightVector
	m_vUpVector = glm::cross(m_vLookVector, m_vRightVector);
	m_vUpVector = glm::normalize(m_vUpVector);
	m_vRightVector = glm::cross(m_vUpVector, m_vLookVector);
	m_vRightVector = glm::normalize(m_vRightVector);
}

//-----------------------------------------------------------------------------
// Name��CameraClass::SetCameraPosition( )
// Desc: ������������ڵ�λ��
//-----------------------------------------------------------------------------
void MagicCamera::SetCameraPosition(glm::vec3 *pVector)
{
	glm::vec3 V = glm::vec3(0.0f, 0.0f, -20.0f);
	m_vCameraPosition = pVector ? (*pVector) : V;//��Ŀ����������pVectorΪ��Ļ���
												 //����*pVector��ֵ����m_vCameraPosition=*pVector����
												 //���򷵻�V��ֵ����m_vCameraPosition=V��
}

//-----------------------------------------------------------------------------
// Name��CameraClass::SetViewMatrix( )
// Desc: ����ȡ���任����
//-----------------------------------------------------------------------------
void MagicCamera::SetViewMatrix(glm::mat4 *pMatrix)
{
	//����pMatrix��ֵ����һ���ж�
	if (pMatrix)
		m_matView = *pMatrix;
	else
		m_matView = CalculateViewMatrix();
	//	m_matView = glm::lookAt(glm::vec3(10.0f, 50.0f, 20.0f), glm::vec3(10.0f, 50.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	//��ȡ���任�����ֵ�������ֱ���ҷ������Ϸ������͹۲����
	m_vRightVector = glm::vec3(m_matView[0].x, m_matView[1].x, m_matView[2].x);
	m_vUpVector = glm::vec3(m_matView[0].y, m_matView[1].y, m_matView[2].y);
	m_vLookVector = glm::vec3(m_matView[0].z, m_matView[1].z, m_matView[2].z);
}

//-----------------------------------------------------------------------------
// Name��CameraClass::SetProjMatrix( )
// Desc: ����ͶӰ�任����
//-----------------------------------------------------------------------------
void MagicCamera::SetProjMatrix(glm::mat4 *pMatrix)
{
	//�ж�ֵ��û�У�û�еĻ��ͼ���һ��
	if (pMatrix != NULL)
		m_matProj = *pMatrix;
	else
		m_matProj = glm::perspective(-70.0f, -1024.0f / 768.0f, 10.0f, 100000.0f);
}

void MagicCamera::SetPosition(float _x, float _y, float _z)
{
	glm::vec3 _pos(_x, _y, _z);
	m_vCameraPosition = _pos;
	_pos -= m_vCameraPosition;
	m_vTargetPosition += _pos;
}

//-----------------------------------------------------------------------------
// Name��CameraClass::MoveAlongRightVec( )
// Desc: ��������ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongRightVec(float fUnits)
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition += m_vRightVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name��CameraClass::MoveAlongUpVec( )
// Desc:  ��������ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongUpVec(float fUnits)
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	/*	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition += m_vUpVector * fUnits;*/
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name��CameraClass::MoveAlongLookVec( )
// Desc:  �ع۲�����ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongLookVec(float fUnits)
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	/*	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;*/
	m_vCameraPosition.z = fUnits;
	m_vTargetPosition.z = fUnits;
}

//-----------------------------------------------------------------------------
// Name��CameraClass::RotationRightVec( )
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void MagicCamera::RotationRightVec(float fAngle)
{
	glm::mat4 R;
	R = glm::rotate(fAngle, m_XYVector);
	m_vUpVector = Magic::WVec3TransformCoord(m_vUpVector, R);//��m_vUpVector������m_vRightVector��תfAngle���Ƕ�
	m_vLookVector = Magic::WVec3TransformCoord(m_vLookVector, R);//��m_vLookVector������m_vRightVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}

//-----------------------------------------------------------------------------
// Name��CameraClass::RotationUpVec( )
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void MagicCamera::RotationUpVec(float fAngle)
{
	glm::mat4 R;
	//	R = glm::rotate(fAngle, m_vUpVector);//��������m_vUpVector��תfAngle���Ƕȵ�R����
	R = glm::rotate(fAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	m_XYVector = Magic::WVec3TransformCoord(m_XYVector, R);
	m_vRightVector = Magic::WVec3TransformCoord(m_vRightVector, R);//��m_vRightVector������m_vUpVector��תfAngle���Ƕ�
	m_vLookVector = Magic::WVec3TransformCoord(m_vLookVector, R);//��m_vLookVector������m_vUpVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}

//-----------------------------------------------------------------------------
// Name��CameraClass::RotationLookVec( )
// Desc:  �ع۲�������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void MagicCamera::RotationLookVec(float fAngle)
{
	glm::mat4 R;
	R = glm::rotate(fAngle, m_vLookVector);//��������m_vLookVector��תfAngle���Ƕȵ�R����
	m_vRightVector = Magic::WVec3TransformCoord(m_vRightVector, R);//��m_vRightVector������m_vLookVector��תfAngle���Ƕ�
	m_vUpVector = Magic::WVec3TransformCoord(m_vUpVector, R);//��m_vUpVector������m_vLookVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}

void MagicCamera::SetRotation(float _Right, float _Look, float _Up)
{
	glm::mat4 RRight, RLook, RUp, R;

	RRight = glm::rotate(_Right, glm::vec3(1.0f, 0.0f, 0.0f));
	RLook = glm::rotate(_Look, glm::vec3(0.0f, 1.0f, 0.0f));
	RUp = glm::rotate(_Up, glm::vec3(0.0f, 0.0f, 1.0f));
	R = RRight * RLook * RUp;
	m_vLookVector = Magic::WVec3TransformCoord(glm::vec3(0.0f, 1.0f, 0.0f), R);
	m_vRightVector = Magic::WVec3TransformCoord(glm::vec3(1.0f, 0.0f, 0.0f), R);
	m_vUpVector = Magic::WVec3TransformCoord(glm::vec3(0.0f, 0.0f, 1.0f), R);

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}
