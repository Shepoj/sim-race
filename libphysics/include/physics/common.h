#pragma once
//#include "tools/Filesystem.h"

#include "physics/platform_core.h"

#include <map>
#include <string>

namespace sim::common
{

constexpr double PI = 3.14159;
constexpr double DEG_TO_RAD = PI / 180.;
constexpr double RAD_TO_DEG = 180. / PI;

struct SIMRACE_DLL Vector2
{
	Vector2() = default;
	Vector2(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	double x {};
	double y {};
};

struct SIMRACE_DLL Vector3
{
	Vector3() = default;
	Vector3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double x {};
	double y {};
	double z {};
};

Vector2 SIMRACE_DLL operator * (const Vector2& vec, double scalar);
Vector2 SIMRACE_DLL operator * (double scalar, const Vector2& vec);

using ForceTorque2D = Vector3;

//Vector2 SIMRACE_DLL toWorld(const Vector2& bodyFrame);
//
//Vector2 SIMRACE_DLL toBody(const Vector2& worldFrame);

Vector2 SIMRACE_DLL rotateVector(const Vector2& direction, double angleRad);

Vector2 SIMRACE_DLL invRotateVector(const Vector2& direction, double angleRad);

Vector2 SIMRACE_DLL worldToScreen(const Vector2& worldPos, Vector2& sizeFactor, const Vector2& offset = Vector2());

}