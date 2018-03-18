#ifndef _MAGIC_MATH_H_
#define _MAGIC_MATH_H_

#pragma once


#include "glm.hpp"
#include "Magic_Macro.h"

#define MAGIC_PI    (3.14159265358979323846)
#define FT_ME    (3.2808398950131)
#define HG_MB    (33.863881400716413680439927997685)

#define EARTH_Radius  (6378137)   //地球半径单位 米

#define RGBcharTofloat(_x)               (_x / 255.0f)

namespace Magic
{
	glm::vec3 WVec3TransformCoord(glm::vec3 Vec, glm::mat4 Maxtrix);

	//计算两个向量的夹角,函数返回一个角度值
	DLL_MAGIC_ENGINE_OUTPUT_INPUT float VectorIncludedAngle(glm::vec3, glm::vec3);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float VectorIncludedAngle(glm::vec2, glm::vec2);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float VectorIncludedRadian(glm::vec3, glm::vec3);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool lat_to_H(double lat_d, double &H);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool H_to_lat(double H, double &lat_d);
	//计算两个经纬度之间的距离，单位海里
	DLL_MAGIC_ENGINE_OUTPUT_INPUT float TwoLatLondistance(glm::vec2 _LatLon1, glm::vec2 _LatLon2);
	//米转换成海里
	DLL_MAGIC_ENGINE_OUTPUT_INPUT float MtoNM(float _M);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT glm::vec3 SphereToDescartes(float _lon, float _lat, float _r);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT glm::vec3 SphereToVector(float _lon, float _lat);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT glm::vec3 SphereToVector(glm::vec2 _lonlat);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT glm::vec2 VectorToSphere(glm::vec3 _vector);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float TEMP_CtoF(float _data);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float TEMP_FtoC(float _data);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float MEtoFT(float _data);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT float FTtoME(float _data);
}
#endif