#include "Engine/Math/ConvexPoly2D.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Math/Plane2.hpp"

ConvexPoly2D::ConvexPoly2D(std::vector<Vec2> const& verts)
	:m_positions(verts)
{
	ComputeBoundingDisc();
}

std::vector<Vec2> const& ConvexPoly2D::GetVerts() const
{
	return m_positions;
}

int ConvexPoly2D::GetVertexCount() const
{
	return static_cast<int>(m_positions.size());
}

Disc2D const& ConvexPoly2D::GetBoundingDisc() const
{
	return m_boundingDisc;
}

bool ConvexPoly2D::IsPointInside(Vec2 const& point) const
{
	int posCount = GetVertexCount();

	// Check if there is a convex shape
	if (posCount < 3)
	{
		return false;
	}

	// Tracking our left/right orientation sign of the point relative to our convex poly edges.
	// Zero means we aren't yet established
	// If we are greater than zero we are consistently on one side, less than we are opposite
	float sign = 0.f;

	// Check against edges
	for (int posIndex = 0; posIndex < posCount; ++posIndex)
	{
		Vec2 const& posA = m_positions[posIndex];
		Vec2 const& posB = m_positions[(posIndex + 1) % posCount];
		Vec2 bToADisp = posB - posA;
		Vec2 aToPoint = point - posA;

		// Here I use cross product to tell us which side of the edge our point lies on
		float crossEdgeToPoint = CrossProduct2D(bToADisp, aToPoint);

		// On edge check, still inside
		if (crossEdgeToPoint == 0.f)
		{
			continue;
		}

		// Our first non-zero cross gives us the inside
		if (sign == 0.f)
		{
			sign = crossEdgeToPoint;
		}
		// Opposite edge, cannot be inside
		else if (sign * crossEdgeToPoint < 0.f)
		{
			return false;
		}
	}

	return true;
}

void ConvexPoly2D::ComputeBoundingDisc()
{
	// Computing disc center as average of positions
	Vec2 discCenter = Vec2::ZERO;
	for (int posIndex = 0; posIndex < static_cast<int>(m_positions.size()); ++posIndex)
	{
		discCenter += m_positions[posIndex];
	}

	discCenter /= static_cast<float>(m_positions.size());

	// Computing radius as max distance from disc center
	float maxDistanceSquared = 0.f;
	for (int posIndex = 0; posIndex < static_cast<int>(m_positions.size()); ++posIndex)
	{
		float distSquared = GetDistanceSquared2D(discCenter, m_positions[posIndex]);
		maxDistanceSquared = GetMax(maxDistanceSquared, distSquared);
	}

	// Storing the bounding disc
	m_boundingDisc.m_discCenter = discCenter;
	m_boundingDisc.m_discRadius = sqrtf(maxDistanceSquared);
}

std::vector<Plane2> ConvexPoly2D::ComputeConvexHullPlanes() const
{
	std::vector<Plane2> planes;

	int posCount = GetVertexCount();
	if (posCount < 3)
	{
		return planes;
	}

	planes.reserve(posCount);
	for (int posIndex = 0; posIndex < posCount; ++posIndex)
	{
		Vec2 const& posA = m_positions[posIndex];
		Vec2 const& posB = m_positions[(posIndex + 1) % posCount];

		Vec2 edge = posB - posA;

		Vec2 normal(edge.y, -edge.x);
		normal.Normalize();

		float distance = DotProduct2D(normal, posA);
		planes.emplace_back(normal, distance);
	}

	return planes;
}
