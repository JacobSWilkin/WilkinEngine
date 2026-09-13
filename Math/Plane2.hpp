#pragma once
#include "Engine/Math/Vec2.hpp"
// -----------------------------------------------------------------------------
struct Plane2
{
public:
	Vec2  m_planeNormal = Vec2::ZERO_TO_ONE;
	float m_distance = 0.f;

public:
	Plane2() = default;
	~Plane2() = default;
	Plane2(Plane2 const& copyFrom) = default;
	explicit Plane2(Vec2 const& normal, float distance);

	bool IsPointInBack(Vec2 const& point) const;
	bool IsPointInFront(Vec2 const& point) const;
	Vec2 GetPlaneCenter() const;
	
	static Plane2 MakePlaneFromTwoPoints(Vec2 const& pointA, Vec2 const& pointB);
};
// -----------------------------------------------------------------------------