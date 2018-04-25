#include "MagicEarth.h"
#include "Include/MagicMath.h"
#include "Physical/Collision.h"

TERRAIN_VEXTER::TERRAIN_VEXTER()
{

}

TERRAIN_VEXTER::TERRAIN_VEXTER(float _x, float _y, float _z, float _u, float _v, float _h) :x(_x), y(_y), z(_z), u(_u), v(_v), h(_h)
{
}

EarthScenes::EarthScenes()
{
	m_Lod_Draw_Level_Max = 0;
	m_IndexSize = 0;
}

void EarthScenes::BuildGrid(int _level, TERRAIN_VEXTER* _V0, TERRAIN_VEXTER* _V1, TERRAIN_VEXTER* _V2, int _cullfags)
{
	//计算分割顶点
	TERRAIN_VEXTER _New_V((_V0->x + _V2->x)* 0.5f, (_V0->y + _V2->y)* 0.5f, (_V0->z + _V2->z)* 0.5f,
		(_V0->u + _V2->u)* 0.5f, (_V0->v + _V2->v)* 0.5f, 0.0f);

	//获取高度
	_New_V.h = 0.0f;

	//重新计算三维坐标
	float _sqr = _New_V.x * _New_V.x + _New_V.y * _New_V.y + _New_V.z * _New_V.z;
	float _inversesqrt = 1.0f / glm::sqrt(_sqr);
	float _height = EARTH_Radius + _New_V.h;

	//检查是否在摄像机内
	float _RC, _RD = 0.0f;
	float _X, _Y, _Z;

	_X = _V0->x - _New_V.x; _Y = _V0->y - _New_V.y; _Z = _V0->z - _New_V.z;
	_RC = _X * _X + _Y * _Y + _Z * _Z;
	if (_RC > _RD) _RD = _RC;
	_X = _V1->x - _New_V.x; _Y = _V1->y - _New_V.y; _Z = _V1->z - _New_V.z;
	_RC = _X * _X + _Y * _Y + _Z * _Z;
	if (_RC > _RD) _RD = _RC;
	_X = _V2->x - _New_V.x; _Y = _V2->y - _New_V.y; _Z = _V2->z - _New_V.z;
	if (_RC > _RD) _RD = _RC;

	if (_cullfags != 0x3f)
	{
		for (int p = 0, m = 1; p < 6; p++, m <<= 1)
		{
			//如果在以前判断过的方向就不再判断
			if (!(_cullfags & m))
			{
				_RC = m_Frustum.frustum[p][0] * _New_V.x +
					m_Frustum.frustum[p][1] * _New_V.y +
					m_Frustum.frustum[p][2] * _New_V.z +
					m_Frustum.frustum[p][3];

				if (_RC * _RC > _RD)
				{
					if (_RC < 0.0f)
						return;
					_cullfags |= m;
				}
			}
		}
	}

	//是否满足切割距离和最大切割等级
	_X = m_CameraPos.x - _New_V.x; _Y = m_CameraPos.y - _New_V.y; _Z = m_CameraPos.z - _New_V.z;
	_RC = _X * _X + _Y * _Y + _Z * _Z;
	if (_level < m_Lod_Draw_Level_Max &&
		V_LevelDrawDistance[_level] > _RC)
	{
		BuildGrid(_level + 1, _V1, &_New_V, _V0, _cullfags);
		BuildGrid(_level + 1, _V2, &_New_V, _V1, _cullfags);
		return;
	}

	pVertex[m_IndexSize++] = *_V0;
	pVertex[m_IndexSize++] = *_V1;
	pVertex[m_IndexSize++] = *_V2;
}

float EarthScenes::FindNowPosHeight(int _level, TERRAIN_VEXTER* _V0, TERRAIN_VEXTER* _V1, TERRAIN_VEXTER* _V2)
{
	//计算分割顶点
	TERRAIN_VEXTER _New_V((_V0->x + _V2->x)* 0.5f, (_V0->y + _V2->y)* 0.5f, (_V0->z + _V2->z)* 0.5f,
		(_V0->u + _V2->u)* 0.5f, (_V0->v + _V2->v)* 0.5f, 0.0f);

	//获取高度
	_New_V.h = 0.0f;

	//重新计算三维坐标
	float _sqr = _New_V.x * _New_V.x + _New_V.y * _New_V.y + _New_V.z * _New_V.z;
	float _inversesqrt = 1.0f / glm::sqrt(_sqr);
	float _height = EARTH_Radius + _New_V.h;

	if (_level < m_Lod_Draw_Level_Max)
	{
		glm::vec3 _endPosNor = glm::cross(glm::vec3(_New_V.x - _V1->x, _New_V.y - _V1->y, _New_V.z - _V1->z),
			glm::vec3(_New_V.x, _New_V.y, _New_V.z));
		if (_endPosNor.x * m_CameraPos.x + _endPosNor.y * m_CameraPos.y + _endPosNor.z * m_CameraPos.z >= 0)
			return FindNowPosHeight(_level + 1, _V1, &_New_V, _V0);
		else
			return FindNowPosHeight(_level + 1, _V2, &_New_V, _V1);
	}

	//显示碰撞区域
	pVertex[m_IndexSize++] = *_V0;
	pVertex[m_IndexSize++] = *_V1;
	pVertex[m_IndexSize++] = *_V2;

	glm::vec3 _Direction = glm::normalize(-m_CameraPos);
	float _pDist = -1.0f;
	bool _result = Magic::IntersectRayTriangle(m_CameraPos, _Direction,
		glm::vec3(_V0->x, _V0->y, _V0->z),
		glm::vec3(_V1->x, _V1->y, _V1->z),
		glm::vec3(_V2->x, _V2->y, _V2->z), &_pDist);
	if (_pDist > 0 && _result)
		return glm::length(Magic::XMVecMultiplayAdd(_Direction, glm::vec3(_pDist), m_CameraPos)) - EARTH_Radius;
	else
		return 0.0f;
}

void EarthScenes::OnUpdata()
{

}

void EarthScenes::Draw()
{

}