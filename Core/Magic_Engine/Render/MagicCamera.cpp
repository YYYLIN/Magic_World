#include "MagicCamera.h"
#include "Math/MagicMath.h"


MagicCamera::MagicCamera()
{
	//	m_pd3dDevice = pd3dDevice;
	m_vRightVector = glm::vec3(1.0f, 0.0f, 0.0f);   // 默认右向量与X正半轴重合
	m_vUpVector = glm::vec3(0.0f, 1.0f, 0.0f);   // 默认上向量与Y正半轴重合
	m_vLookVector = glm::vec3(0.0f, 0.0f, 1.0f);   // 默认观察向量与Z正半轴重合
	m_vCameraPosition = glm::vec3(0.0f, 0.0f, -20.0f);   // 默认摄像机坐标为(0.0f, 0.0f, -250.0f)
	m_vTargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);//默认观察目标位置为(0.0f, 0.0f, 0.0f);
	m_XYVector = glm::vec3(1.0f, 0.0f, 0.0f);
}


MagicCamera::~MagicCamera()
{
}


//-----------------------------------------------------------------------------
// Name：CameraClass::CalculateViewMatrix( )
// Desc: 根据给定的矩阵计算出取景变换矩阵
//-----------------------------------------------------------------------------
glm::mat4 MagicCamera::CalculateViewMatrix()
{
	glm::mat4 pMatrix;
	//1.先把3个向量都规范化并使其相互垂直，成为一组正交矩阵
	m_vLookVector = glm::normalize(m_vLookVector);//规范化观察分量
	m_vUpVector = glm::cross(m_vLookVector, m_vRightVector); // 上向量与观察向量垂直
	m_vUpVector = glm::normalize(m_vUpVector); // 规范化上向量              
	m_vRightVector = glm::cross(m_vUpVector, m_vLookVector);	// 右向量与上向量垂直
	m_vRightVector = glm::normalize(m_vRightVector); // 规范化右向量


													 // 2.创建出取景变换矩阵
													 //依次写出取景变换矩阵的第一行
	pMatrix[0].x = m_vRightVector.x;           // Rx
	pMatrix[0].y = m_vUpVector.x;              // Ux
	pMatrix[0].z = m_vLookVector.x;            // Lx
	pMatrix[0].w = 0.0f;
	//依次写出取景变换矩阵的第二行
	pMatrix[1].x = m_vRightVector.y;           // Ry
	pMatrix[1].y = m_vUpVector.y;              // Uy
	pMatrix[1].z = m_vLookVector.y;            // Ly
	pMatrix[1].w = 0.0f;
	//依次写出取景变换矩阵的第三行
	pMatrix[2].x = m_vRightVector.z;           // Rz
	pMatrix[2].y = m_vUpVector.z;              // Uz
	pMatrix[2].z = m_vLookVector.z;            // Lz
	pMatrix[2].w = 0.0f;
	//依次写出取景变换矩阵的第四行

	pMatrix[3].x = -glm::dot(m_vRightVector, m_vCameraPosition);         // -P*R
	pMatrix[3].y = -glm::dot(m_vUpVector, m_vCameraPosition);       // -P*U
	pMatrix[3].z = -glm::dot(m_vLookVector, m_vCameraPosition);     // -P*L
	pMatrix[3].w = 1.0f;

	return pMatrix;
}


//-----------------------------------------------------------------------------
// Name：CameraClass::SetTargetPosition( )
// Desc: 设置摄像机的观察位置
//-----------------------------------------------------------------------------
void MagicCamera::SetTargetPosition(glm::vec3 *pLookat)
{
	//先看看pLookat是否为默认值NULL
	if (pLookat != NULL)
		m_vTargetPosition = (*pLookat);
	else
		m_vTargetPosition = glm::vec3(0.0f, 0.0f, 1.0f);

	m_vLookVector = m_vTargetPosition - m_vCameraPosition;//观察点位置减摄像机位置，得到观察方向向量
	m_vLookVector = glm::normalize(m_vLookVector);//规范化m_vLookVector向量

												  //正交并规范化m_vUpVector和m_vRightVector
	m_vUpVector = glm::cross(m_vLookVector, m_vRightVector);
	m_vUpVector = glm::normalize(m_vUpVector);
	m_vRightVector = glm::cross(m_vUpVector, m_vLookVector);
	m_vRightVector = glm::normalize(m_vRightVector);
}

//-----------------------------------------------------------------------------
// Name：CameraClass::SetCameraPosition( )
// Desc: 设置摄像机所在的位置
//-----------------------------------------------------------------------------
void MagicCamera::SetCameraPosition(glm::vec3 *pVector)
{
	glm::vec3 V = glm::vec3(0.0f, 0.0f, -20.0f);
	m_vCameraPosition = pVector ? (*pVector) : V;//三目运算符，如果pVector为真的话，
												 //返回*pVector的值（即m_vCameraPosition=*pVector），
												 //否则返回V的值（即m_vCameraPosition=V）
}

//-----------------------------------------------------------------------------
// Name：CameraClass::SetViewMatrix( )
// Desc: 设置取景变换矩阵
//-----------------------------------------------------------------------------
void MagicCamera::SetViewMatrix(glm::mat4 *pMatrix)
{
	//根据pMatrix的值先做一下判断
	if (pMatrix)
		m_matView = *pMatrix;
	else
		m_matView = CalculateViewMatrix();
	//	m_matView = glm::lookAt(glm::vec3(10.0f, 50.0f, 20.0f), glm::vec3(10.0f, 50.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	//把取景变换矩阵的值分下来分别给右分量，上分量，和观察分量
	m_vRightVector = glm::vec3(m_matView[0].x, m_matView[1].x, m_matView[2].x);
	m_vUpVector = glm::vec3(m_matView[0].y, m_matView[1].y, m_matView[2].y);
	m_vLookVector = glm::vec3(m_matView[0].z, m_matView[1].z, m_matView[2].z);
}

//-----------------------------------------------------------------------------
// Name：CameraClass::SetProjMatrix( )
// Desc: 设置投影变换矩阵
//-----------------------------------------------------------------------------
void MagicCamera::SetProjMatrix(glm::mat4 *pMatrix)
{
	//判断值有没有，没有的话就计算一下
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
// Name：CameraClass::MoveAlongRightVec( )
// Desc: 沿右向量平移fUnits个单位
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongRightVec(float fUnits)
{
	//直接乘以fUnits的量来累加就行了
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition += m_vRightVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name：CameraClass::MoveAlongUpVec( )
// Desc:  沿上向量平移fUnits个单位
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongUpVec(float fUnits)
{
	//直接乘以fUnits的量来累加就行了
	/*	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition += m_vUpVector * fUnits;*/
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name：CameraClass::MoveAlongLookVec( )
// Desc:  沿观察向量平移fUnits个单位
//-----------------------------------------------------------------------------
void MagicCamera::MoveAlongLookVec(float fUnits)
{
	//直接乘以fUnits的量来累加就行了
	/*	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;*/
	m_vCameraPosition.z = fUnits;
	m_vTargetPosition.z = fUnits;
}

//-----------------------------------------------------------------------------
// Name：CameraClass::RotationRightVec( )
// Desc:  沿右向量旋转fAngle个弧度单位的角度
//-----------------------------------------------------------------------------
void MagicCamera::RotationRightVec(float fAngle)
{
	glm::mat4 R;
	R = glm::rotate(fAngle, m_XYVector);
	m_vUpVector = Magic::WVec3TransformCoord(m_vUpVector, R);//让m_vUpVector向量绕m_vRightVector旋转fAngle个角度
	m_vLookVector = Magic::WVec3TransformCoord(m_vLookVector, R);//让m_vLookVector向量绕m_vRightVector旋转fAngle个角度

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//更新一下观察点的新位置（方向乘以模=向量）
}

//-----------------------------------------------------------------------------
// Name：CameraClass::RotationUpVec( )
// Desc:  沿上向量旋转fAngle个弧度单位的角度
//-----------------------------------------------------------------------------
void MagicCamera::RotationUpVec(float fAngle)
{
	glm::mat4 R;
	//	R = glm::rotate(fAngle, m_vUpVector);//创建出绕m_vUpVector旋转fAngle个角度的R矩阵
	R = glm::rotate(fAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	m_XYVector = Magic::WVec3TransformCoord(m_XYVector, R);
	m_vRightVector = Magic::WVec3TransformCoord(m_vRightVector, R);//让m_vRightVector向量绕m_vUpVector旋转fAngle个角度
	m_vLookVector = Magic::WVec3TransformCoord(m_vLookVector, R);//让m_vLookVector向量绕m_vUpVector旋转fAngle个角度

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//更新一下观察点的新位置（方向乘以模=向量）
}

//-----------------------------------------------------------------------------
// Name：CameraClass::RotationLookVec( )
// Desc:  沿观察向量旋转fAngle个弧度单位的角度
//-----------------------------------------------------------------------------
void MagicCamera::RotationLookVec(float fAngle)
{
	glm::mat4 R;
	R = glm::rotate(fAngle, m_vLookVector);//创建出绕m_vLookVector旋转fAngle个角度的R矩阵
	m_vRightVector = Magic::WVec3TransformCoord(m_vRightVector, R);//让m_vRightVector向量绕m_vLookVector旋转fAngle个角度
	m_vUpVector = Magic::WVec3TransformCoord(m_vUpVector, R);//让m_vUpVector向量绕m_vLookVector旋转fAngle个角度

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//更新一下观察点的新位置（方向乘以模=向量）
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

	m_vTargetPosition = m_vLookVector * glm::length(m_vCameraPosition);//更新一下观察点的新位置（方向乘以模=向量）
}
