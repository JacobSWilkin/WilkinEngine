#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/MathUtils.h"

Plane2::Plane2(Vec2 const& normal, float distance)
	:m_planeNormal(normal), 
	 m_distance(distance)
{
}

bool Plane2::IsPointInBack(Vec2 const& point) const
{
	return DotProduct2D(m_planeNormal, point) < m_distance;
}

bool Plane2::IsPointInFront(Vec2 const& point) const
{
	return DotProduct2D(m_planeNormal, point) > m_distance;
}

Vec2 Plane2::GetPlaneCenter() const
{
	return m_planeNormal * m_distance;
}

Plane2 Plane2::MakePlaneFromTwoPoints(Vec2 const& pointA, Vec2 const& pointB)
{
	Vec2 planeLength = pointB - pointA;
	Vec2 planeNormal = planeLength.GetRotated90Degrees().GetNormalized();
	float distance = DotProduct2D(planeNormal, pointA);
	return Plane2(planeNormal, distance);
}
