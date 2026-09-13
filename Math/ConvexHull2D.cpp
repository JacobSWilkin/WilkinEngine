#include "Engine/Math/ConvexHull2D.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/ConvexPoly2D.hpp"
#include "Engine/Math/MathUtils.h"

ConvexHull2D::ConvexHull2D(std::vector<Plane2> const& planes)
	:m_boundingPlanes(planes)
{
}

size_t ConvexHull2D::GetPlaneCount() const
{
	return m_boundingPlanes.size();
}

bool ConvexHull2D::IsPointInside(Vec2 const& point) const
{
	for (int planeIndex = 0; planeIndex < static_cast<int>(m_boundingPlanes.size()); ++planeIndex)
	{
		Plane2 const& plane = m_boundingPlanes[planeIndex];
		if (plane.IsPointInFront(point))
		{
			return false;
		}
	}
	return true;
}

ConvexHull2D ConvexHull2D::MakeHullFromPoly(ConvexPoly2D const& convexPoly)
{
	std::vector<Plane2> planes;
	std::vector<Vec2> const& verts = convexPoly.GetVerts();
	size_t count = verts.size();

	planes.reserve(count);

	for (size_t posIndex = 0; posIndex < count; ++posIndex)
	{
		Vec2 const& posA = verts[posIndex];
		Vec2 const& posB = verts[(posIndex + 1) % count];
		Vec2 aToBDisplacement = posB - posA;

		Vec2 normal = Vec2(aToBDisplacement.y, -aToBDisplacement.x);
		normal.Normalize();

		float distance = DotProduct2D(normal, posA);

		planes.emplace_back(normal, distance);
	}

	return ConvexHull2D(planes);
}
