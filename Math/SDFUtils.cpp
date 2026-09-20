#include "Engine/Math/SDFUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.h"

float SDFCircle(Vec2 const& point, float radius)
{
	float pLength = point.GetLength();
	return pLength - radius;
}

float SDFAABB(Vec2 const& point, Vec2 const& halfDimensions)
{
	Vec2 dist = Abs(point) - halfDimensions;
	return GetMax(dist, Vec2::ZERO).GetLength() + GetMin(GetMax(dist.x, dist.y), 0.f);
}
