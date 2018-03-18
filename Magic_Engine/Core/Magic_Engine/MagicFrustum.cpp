////////////////////////////////////////////////////////////////////////////////
// Filename: frustumclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MagicFrustum.h"

namespace Magic
{

	float WWTPlaneDotCoord(glm::vec4* _Vec4, glm::vec3* _Vec3)
	{
		return _Vec4->x * _Vec3->x + _Vec4->y * _Vec3->y + _Vec4->z * _Vec3->z + _Vec4->w;
	}

	void WWTPlaneNormalize(glm::vec4* _Vect4)
	{
		float t = sqrt(_Vect4->x*_Vect4->x + _Vect4->y*_Vect4->y + _Vect4->z*_Vect4->z);

		_Vect4->x /= t;
		_Vect4->y /= t;
		_Vect4->z /= t;
		_Vect4->w /= t;
	}

	Frustum::Frustum()
	{
	}


	Frustum::Frustum(const Frustum& other)
	{
	}


	Frustum::~Frustum()
	{
	}


	void Frustum::ConstructFrustum(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		/*	float zMinimum, r;
			glm::mat4 matrix;

			// Calculate the minimum Z distance in the frustum.
			//计算最小截锥体Z的最小距离
			zMinimum = -projectionMatrix[3].z / projectionMatrix[2].z;
			r = screenDepth / (screenDepth - zMinimum);
			projectionMatrix[2].z = r;
			projectionMatrix[3].z = -r * zMinimum;

			// Create the frustum matrix from the view matrix and updated projection matrix.
			//创建截矩阵从视图矩阵和投影矩阵的更新。
			matrix = viewMatrix * projectionMatrix;

			// Calculate near plane of frustum.
			//计算近平面体。
			m_planes[0].x = matrix[3].x + matrix[2].x;
			m_planes[0].y = matrix[3].y + matrix[2].y;
			m_planes[0].z = matrix[3].z + matrix[2].z;
			m_planes[0].w = matrix[3].w + matrix[2].w;
			//	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);//单位化平面向量
			WWTPlaneNormalize(&m_planes[0]);

			// Calculate far plane of frustum.
			//计算体远平面。
			m_planes[1].x = matrix[3].x - matrix[2].x;
			m_planes[1].y = matrix[3].y - matrix[2].y;
			m_planes[1].z = matrix[3].z - matrix[2].z;
			m_planes[1].w = matrix[3].w - matrix[2].w;
			//	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);
			WWTPlaneNormalize(&m_planes[1]);

			// Calculate left plane of frustum.
			//计算体左平面。
			m_planes[2].x = matrix[3].x + matrix[0].x;
			m_planes[2].y = matrix[3].y + matrix[0].y;
			m_planes[2].z = matrix[3].z + matrix[0].z;
			m_planes[2].w = matrix[3].w + matrix[0].w;
			//	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);
			WWTPlaneNormalize(&m_planes[2]);

			// Calculate right plane of frustum.
			//计算体右平面。
			m_planes[3].x = matrix[3].x - matrix[0].x;
			m_planes[3].y = matrix[3].y - matrix[0].y;
			m_planes[3].z = matrix[3].z - matrix[0].z;
			m_planes[3].w = matrix[3].w - matrix[0].w;
			//	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);
			WWTPlaneNormalize(&m_planes[3]);

			// Calculate top plane of frustum.
			//计算体顶面。
			m_planes[4].x = matrix[3].x - matrix[1].x;
			m_planes[4].y = matrix[3].y - matrix[1].y;
			m_planes[4].z = matrix[3].z - matrix[1].z;
			m_planes[4].w = matrix[3].w - matrix[1].w;
			//	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);
			WWTPlaneNormalize(&m_planes[4]);

			// Calculate bottom plane of frustum.
			//计算锥底平面。
			m_planes[5].x = matrix[3].x + matrix[1].x;
			m_planes[5].y = matrix[3].y + matrix[1].y;
			m_planes[5].z = matrix[3].z + matrix[1].z;
			m_planes[5].w = matrix[3].w + matrix[1].w;
			//	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
			WWTPlaneNormalize(&m_planes[5]);*/

		float proj[16];
		float modl[16];
		float clip[16];
		float t;

		for (int a = 0; a < 4; a++)
		{
			/* Get the current PROJECTION matrix from OpenGL */
			proj[a * 4] = projectionMatrix[a].x;
			proj[1 + a * 4] = projectionMatrix[a].y;
			proj[2 + a * 4] = projectionMatrix[a].z;
			proj[3 + a * 4] = projectionMatrix[a].w;

			/*		proj[a] = projectionMatrix[a].x;
					proj[a + 4] = projectionMatrix[a].y;
					proj[a + 8] = projectionMatrix[a].z;
					proj[a + 12] = projectionMatrix[a].w;*/

					/* Get the current MODELVIEW matrix from OpenGL */
			modl[a * 4] = viewMatrix[a].x;
			modl[1 + a * 4] = viewMatrix[a].y;
			modl[2 + a * 4] = viewMatrix[a].z;
			modl[3 + a * 4] = viewMatrix[a].w;

			/*		modl[a] = viewMatrix[a].x;
					modl[a + 4] = viewMatrix[a].y;
					modl[a + 8] = viewMatrix[a].z;
					modl[a + 12] = viewMatrix[a].w;*/
		}

		/* Combine the two matrices (multiply projection by modelview)    */
		clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
		clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
		clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
		clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];
		clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
		clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
		clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
		clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];
		clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
		clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
		clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];
		clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];
		/* Extract the numbers for the RIGHT plane 右平面*/
		frustum[0][0] = clip[3] - clip[0];
		frustum[0][1] = clip[7] - clip[4];
		frustum[0][2] = clip[11] - clip[8];
		frustum[0][3] = clip[15] - clip[12];
		/* Normalize the result */
		t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
		frustum[0][0] /= t;
		frustum[0][1] /= t;
		frustum[0][2] /= t;
		frustum[0][3] /= t;
		/* Extract the numbers for the LEFT plane 左平面*/
		frustum[1][0] = clip[3] + clip[0];
		frustum[1][1] = clip[7] + clip[4];
		frustum[1][2] = clip[11] + clip[8];
		frustum[1][3] = clip[15] + clip[12];
		/* Normalize the result */
		t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
		frustum[1][0] /= t;
		frustum[1][1] /= t;
		frustum[1][2] /= t;
		frustum[1][3] /= t;
		/* Extract the BOTTOM plane  低平面*/
		frustum[2][0] = clip[3] + clip[1];
		frustum[2][1] = clip[7] + clip[5];
		frustum[2][2] = clip[11] + clip[9];
		frustum[2][3] = clip[15] + clip[13];
		/* Normalize the result */
		t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
		frustum[2][0] /= t;
		frustum[2][1] /= t;
		frustum[2][2] /= t;
		frustum[2][3] /= t;
		/* Extract the TOP plane 上平面*/
		frustum[3][0] = clip[3] - clip[1];
		frustum[3][1] = clip[7] - clip[5];
		frustum[3][2] = clip[11] - clip[9];
		frustum[3][3] = clip[15] - clip[13];
		/* Normalize the result */
		t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
		frustum[3][0] /= t;
		frustum[3][1] /= t;
		frustum[3][2] /= t;
		frustum[3][3] /= t;
		/* Extract the FAR plane 远平面*/
		frustum[4][0] = clip[3] - clip[2];
		frustum[4][1] = clip[7] - clip[6];
		frustum[4][2] = clip[11] - clip[10];
		frustum[4][3] = clip[15] - clip[14];
		/* Normalize the result */
		t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
		frustum[4][0] /= t;
		frustum[4][1] /= t;
		frustum[4][2] /= t;
		frustum[4][3] /= t;
		/* Extract the NEAR plane 近平面*/
		frustum[5][0] = clip[3] + clip[2];
		frustum[5][1] = clip[7] + clip[6];
		frustum[5][2] = clip[11] + clip[10];
		frustum[5][3] = clip[15] + clip[14];
		/* Normalize the result */
		t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
		frustum[5][0] /= t;
		frustum[5][1] /= t;
		frustum[5][2] /= t;
		frustum[5][3] /= t;

/*
		Max.x = -(frustum[0][3] / frustum[0][0]);
		Min.x = -(frustum[1][3] / frustum[1][0]);
		Max.y = -(frustum[2][3] / frustum[2][1]);
		Min.y = -(frustum[3][3] / frustum[3][1]);*/

		return;
	}


	bool Frustum::CheckPoint(float x, float y, float z)
	{
		/*	int i;

		// Check if the point is inside all six planes of the view frustum.
		//检查点是在所有6个方向的平面
		for (i = 0; i < 6; i++)
		{
		if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3(x, y, z)) < 0.0f)
		{
		return false;
		}
		}

		return true;*/

		int p;
		for (p = 0; p < 6; p++)
			if (frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3] <= 0)
				return false;
		return true;
	}


	bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
	{
		int p;
		for (p = 0; p < 6; p++)
		{
			if (frustum[p][0] * (xCenter - radius) + frustum[p][1] * (yCenter - radius) + frustum[p][2] * (zCenter - radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + radius) + frustum[p][1] * (yCenter - radius) + frustum[p][2] * (zCenter - radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - radius) + frustum[p][1] * (yCenter + radius) + frustum[p][2] * (zCenter - radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + radius) + frustum[p][1] * (yCenter + radius) + frustum[p][2] * (zCenter - radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - radius) + frustum[p][1] * (yCenter - radius) + frustum[p][2] * (zCenter + radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + radius) + frustum[p][1] * (yCenter - radius) + frustum[p][2] * (zCenter + radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - radius) + frustum[p][1] * (yCenter + radius) + frustum[p][2] * (zCenter + radius) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + radius) + frustum[p][1] * (yCenter + radius) + frustum[p][2] * (zCenter + radius) + frustum[p][3] > 0)
				continue;
			return false;
		}

		return true;
	}


	bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
	{
		int p;
		for (p = 0; p < 6; p++)
			if (frustum[p][0] * xCenter + frustum[p][1] * yCenter + frustum[p][2] * zCenter + frustum[p][3] <= -radius)
				return false;
		return true;
	}


	bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		/*	int i;


			// Check if any of the 6 planes of the rectangle are inside the view frustum.
			for (i = 0; i < 6; i++)
			{
			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			{
			continue;
			}

			if (WWTPlaneDotCoord(&m_planes[i], &glm::vec3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			{
			continue;
			}

			return false;
			}*/

		int p;
		for (p = 0; p < 6; p++)
		{
			if (frustum[p][0] * (xCenter - xSize) + frustum[p][1] * (yCenter - ySize) + frustum[p][2] * (zCenter - zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + xSize) + frustum[p][1] * (yCenter - ySize) + frustum[p][2] * (zCenter - zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - xSize) + frustum[p][1] * (yCenter + ySize) + frustum[p][2] * (zCenter - zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + xSize) + frustum[p][1] * (yCenter + ySize) + frustum[p][2] * (zCenter - zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - xSize) + frustum[p][1] * (yCenter - ySize) + frustum[p][2] * (zCenter + zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + xSize) + frustum[p][1] * (yCenter - ySize) + frustum[p][2] * (zCenter + zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter - xSize) + frustum[p][1] * (yCenter + ySize) + frustum[p][2] * (zCenter + zSize) + frustum[p][3] > 0)
				continue;
			if (frustum[p][0] * (xCenter + xSize) + frustum[p][1] * (yCenter + ySize) + frustum[p][2] * (zCenter + zSize) + frustum[p][3] > 0)
				continue;
			return false;
		}

		return true;
	}

/*
	bool Frustum::CheckPlaneLine(glm::vec2& _Point1, glm::vec2& _Point2, glm::vec3& _normal)
	{
		float result1 = Min.x * _normal.x + Min.y * _normal.y + _normal.z;
		float result2 = Min.x * _normal.x + Max.y * _normal.y + _normal.z;
		float result3 = Max.x * _normal.x + Min.y * _normal.y + _normal.z;
		float result4 = Max.x * _normal.x + Max.y * _normal.y + _normal.z;
		if ((result1 <= 0 && result2 <= 0 && result3 <= 0 && result4 <= 0) ||
			(result1 > 0 && result2 > 0 && result3 > 0 && result4 > 0))
			return false;

		for (int p = 0; p < 4; p++)
			if (frustum[p][0] * _Point1.x + frustum[p][1] * _Point1.y + frustum[p][3] <= 0 &&
				frustum[p][0] * _Point2.x + frustum[p][1] * _Point2.y + frustum[p][3] <= 0)
				return false;
		return true;
	}

	void Frustum::GetMapArea(glm::vec2* _min, glm::vec2* _max)
	{
		*_min = Min;
		*_max = Max;
	}*/
};