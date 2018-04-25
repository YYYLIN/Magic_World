#ifndef _MAGIC_EARTH_H_
#define _MAGIC_EARTH_H

#include <vector>
#include "glm.hpp"
#include "MagicFrustum.h"
#include "Include/MagicEngineContext.h"

struct TERRAIN_VEXTER
{
	float x, y, z;
	float u, v, h;
	TERRAIN_VEXTER();
	TERRAIN_VEXTER(float _x, float _y, float _z, float _u, float _v, float _h);
};

class EarthScenes :public MagicCommon
{
	EarthScenes();

protected:
	void BuildGrid(int _level, TERRAIN_VEXTER* _VO, TERRAIN_VEXTER* _V1, TERRAIN_VEXTER* _V2, int _cullfags);
	float FindNowPosHeight(int _level, TERRAIN_VEXTER* _V0, TERRAIN_VEXTER* _V1, TERRAIN_VEXTER* _V2);

	virtual void OnUpdata() override;
	virtual	void Draw() override;
private:
	int m_Lod_Draw_Level_Max;
	glm::vec3 m_CameraPos;
	Magic::Frustum m_Frustum;
	std::vector<int> V_LevelDrawDistance;


	TERRAIN_VEXTER* pVertex;
	unsigned int m_IndexSize;
};


#endif