#pragma once

#include "Magic_Macro.h"
#include "glm.hpp"


glm::vec3 WVec3TransformCoord(glm::vec3 Vec, glm::mat4 Maxtrix);

//�������������ļн�,��������һ���Ƕ�ֵ
__declspec (dllexport) float VectorIncludedAngle(glm::vec3, glm::vec3);

__declspec (dllexport) float VectorIncludedAngle(glm::vec2, glm::vec2);

__declspec (dllexport) float VectorIncludedRadian(glm::vec3, glm::vec3);

__declspec (dllexport) bool lat_to_H(double lat_d, double &H);

__declspec (dllexport) bool H_to_lat(double H, double &lat_d);
//����������γ��֮��ľ��룬��λ����
__declspec (dllexport) float TwoLatLondistance(glm::vec2 _LatLon1, glm::vec2 _LatLon2);
//��ת���ɺ���
__declspec (dllexport) float MtoNM(float _M);

__declspec (dllexport) glm::vec3 SphereToDescartes(float _lon, float _lat, float _r);

__declspec (dllexport) glm::vec3 SphereToVector(float _lon, float _lat);

__declspec (dllexport) glm::vec3 SphereToVector(glm::vec2 _lonlat);

__declspec (dllexport) glm::vec2 VectorToSphere(glm::vec3 _vector);

__declspec (dllexport) float TEMP_CtoF(float _data);

__declspec (dllexport) float TEMP_FtoC(float _data);

__declspec (dllexport) float MEtoFT(float _data);

__declspec (dllexport) float FTtoME(float _data);