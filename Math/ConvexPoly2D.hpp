#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct Vec2;
struct Plane2;
// -----------------------------------------------------------------------------
struct Disc2D
{
	Vec2  m_discCenter = Vec2::ZERO;
	float m_discRadius = 0.f;
};
// -----------------------------------------------------------------------------
class ConvexPoly2D
{
public:
	// Constructors
	ConvexPoly2D() = default;
	explicit ConvexPoly2D(std::vector<Vec2> const& verts);

	// Accessors
	std::vector<Vec2> const& GetVerts() const;
	int GetVertexCount() const;
	Disc2D const& GetBoundingDisc() const;
	bool IsPointInside(Vec2 const& point) const;

	void ComputeBoundingDisc();
	std::vector<Plane2> ComputeConvexHullPlanes() const;

private:
	std::vector<Vec2> m_positions;
	Disc2D            m_boundingDisc;
};
// -----------------------------------------------------------------------------