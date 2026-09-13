#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct Plane2;
class ConvexPoly2D;
// -----------------------------------------------------------------------------
class ConvexHull2D
{
public:
	// Constructors
	ConvexHull2D() = default;
	explicit ConvexHull2D(std::vector<Plane2> const& planes);

	// Accessors
	size_t GetPlaneCount() const;
	bool   IsPointInside(Vec2 const& point) const;

	// Conversion
	static ConvexHull2D MakeHullFromPoly(ConvexPoly2D const& convexPoly);

public:
	std::vector<Plane2> m_boundingPlanes;
};
// -----------------------------------------------------------------------------