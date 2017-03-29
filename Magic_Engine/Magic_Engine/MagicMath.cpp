#include "MagicMath.h"


glm::vec3 WVec3TransformCoord(glm::vec3 Vec, glm::mat4 Maxtrix)
{
	glm::vec3 Vout;

	Vout.x = Vec.x*Maxtrix[0].x + Vec.y*Maxtrix[0].y + Vec.z*Maxtrix[0].z + Maxtrix[0].w;
	Vout.y = Vec.x*Maxtrix[1].x + Vec.y*Maxtrix[1].y + Vec.z*Maxtrix[1].z + Maxtrix[1].w;
	Vout.z = Vec.x*Maxtrix[2].x + Vec.y*Maxtrix[2].y + Vec.z*Maxtrix[2].z + Maxtrix[2].w;

	return Vout;
}


__declspec (dllexport) float VectorIncludedAngle(glm::vec3 vector1, glm::vec3 vector2)
{
	return (180 / (float)MAGIC_PI)*
		acos(
			glm::dot(vector1, vector2) /
			(glm::length(vector1) * glm::length(vector2))
			);
}

__declspec (dllexport) float VectorIncludedAngle(glm::vec2 vector1, glm::vec2 vector2)
{
	return (180 / (float)MAGIC_PI)*
		acos(
			glm::dot(vector1, vector2) /
			(glm::length(vector1) * glm::length(vector2))
			);
}

__declspec (dllexport) float VectorIncludedRadian(glm::vec3 vector1, glm::vec3 vector2)
{
	return acos(
		glm::dot(vector1, vector2) /
		(glm::length(vector1) * glm::length(vector2))
		);
}

__declspec (dllexport) bool lat_to_H(double lat_d, double &H)
{
	const double PI = 3.141592653;
	if (lat_d > 90 || lat_d < -90 || (&H) == NULL)
		return false;
	double lat_r = lat_d / 180 * PI;
	if (lat_r > 0)
		H = log(fabs(1.0 / cos(fabs(lat_r)) + tan(fabs(lat_r)))) * 180 / PI;
	else
		H = -log(fabs(1.0 / cos(fabs(lat_r)) + tan(fabs(lat_r)))) * 180 / PI;
	return true;
}


__declspec (dllexport) bool H_to_lat(double H, double &lat_d)
{
	const double PI = 3.141592653;
	double A2 = 0.0;
	double lat_r = 0.0;
	if ((&lat_d) == NULL)
	{
		return false;
	}
	A2 = (exp(fabs(H)*PI / 180)) * (exp(fabs(H)*PI / 180));
	if (H >= 0)
	{
		lat_r = asin((A2 - 1) / (A2 + 1));
	}
	else
	{
		lat_r = 0.0 - asin((A2 - 1) / (A2 + 1));
	}
	lat_d = lat_r / PI * 180;
	return true;
}

__declspec (dllexport) float TwoLatLondistance(glm::vec2 _LatLon1, glm::vec2 _LatLon2)
{
	return MtoNM(VectorIncludedRadian(SphereToVector(_LatLon1), SphereToVector(_LatLon2)) * EARTH_Radius);
}

__declspec (dllexport) float MtoNM(float _M)
{
	return _M /= 1852.0f;
}

__declspec (dllexport) glm::vec3 SphereToDescartes(float _lon, float _lat, float _r)
{
	_lon = glm::radians(_lon);
	_lat = glm::radians(_lat);
	float _sinlon = glm::sin(_lon);
	float _coslon = glm::cos(_lon);
	float _sinlat = glm::sin(_lat);
	float _coslat = glm::cos(_lat);
	return _r * glm::vec3(_sinlon * _coslat, _coslon * _coslat, _sinlat);
}

__declspec (dllexport) glm::vec3 SphereToVector(float _lon, float _lat)
{
	_lon = glm::radians(_lon);
	_lat = glm::radians(_lat);
	float _sinlon = glm::sin(_lon);
	float _coslon = glm::cos(_lon);
	float _sinlat = glm::sin(_lat);
	float _coslat = glm::cos(_lat);
	return glm::vec3(_sinlon * _coslat, _coslon * _coslat, _sinlat);
}

__declspec (dllexport) glm::vec3 SphereToVector(glm::vec2 _lonlat)
{
	_lonlat.x = glm::radians(_lonlat.x);
	_lonlat.y = glm::radians(_lonlat.y);
	float _sinlon = glm::sin(_lonlat.x);
	float _coslon = glm::cos(_lonlat.x);
	float _sinlat = glm::sin(_lonlat.y);
	float _coslat = glm::cos(_lonlat.y);
	return glm::vec3(_sinlon * _coslat, _coslon * _coslat, _sinlat);
}

__declspec (dllexport) glm::vec2 VectorToSphere(glm::vec3 _vector)
{
	float _lon = abs(glm::atan(_vector.x / _vector.y));
	_lon = _vector.y < 0.0f ? (float)MAGIC_PI - _lon : _lon;
	_lon = _vector.x < 0.0f ? -_lon : _lon;
	return (180 / (float)MAGIC_PI) * glm::vec2(_lon, glm::asin(_vector.z / glm::length(_vector)));
}

__declspec (dllexport) float TEMP_CtoF(float _data)
{
	return _data * 1.8f + 32;
}

__declspec (dllexport) float TEMP_FtoC(float _data)
{
	return (_data - 32) * 5 / 9;
}

__declspec (dllexport) float MEtoFT(float _data)
{
	return _data * (float)FT_ME;
}

__declspec (dllexport) float FTtoME(float _data)
{
	return _data* 0.3048f;
}