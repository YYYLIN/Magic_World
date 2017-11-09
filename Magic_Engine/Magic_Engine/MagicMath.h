#pragma once

#include "glm.hpp"

#define MAGIC_PI    (3.14159265358979323846)
#define FT_ME    (3.2808398950131)
#define HG_MB    (33.863881400716413680439927997685)

#define EARTH_Radius  (6378137)   //地球半径单位 米

#define RGBcharTofloat(_x)               (_x / 255.0f)

glm::vec3 WVec3TransformCoord(glm::vec3 Vec, glm::mat4 Maxtrix);

//计算两个向量的夹角,函数返回一个角度值
__declspec (dllexport) float VectorIncludedAngle(glm::vec3, glm::vec3);

__declspec (dllexport) float VectorIncludedAngle(glm::vec2, glm::vec2);

__declspec (dllexport) float VectorIncludedRadian(glm::vec3, glm::vec3);

__declspec (dllexport) bool lat_to_H(double lat_d, double &H);

__declspec (dllexport) bool H_to_lat(double H, double &lat_d);
//计算两个经纬度之间的距离，单位海里
__declspec (dllexport) float TwoLatLondistance(glm::vec2 _LatLon1, glm::vec2 _LatLon2);
//米转换成海里
__declspec (dllexport) float MtoNM(float _M);

__declspec (dllexport) glm::vec3 SphereToDescartes(float _lon, float _lat, float _r);

__declspec (dllexport) glm::vec3 SphereToVector(float _lon, float _lat);

__declspec (dllexport) glm::vec3 SphereToVector(glm::vec2 _lonlat);

__declspec (dllexport) glm::vec2 VectorToSphere(glm::vec3 _vector);

__declspec (dllexport) float TEMP_CtoF(float _data);

__declspec (dllexport) float TEMP_FtoC(float _data);

__declspec (dllexport) float MEtoFT(float _data);

__declspec (dllexport) float FTtoME(float _data);