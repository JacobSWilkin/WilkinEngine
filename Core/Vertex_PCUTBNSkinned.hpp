#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/IntVec4.hpp"
#include "Engine/Core/Rgba8.h"
// -----------------------------------------------------------------------------
struct Vertex_PCUTBNSkinned
{
	Vertex_PCUTBNSkinned() = default;
	Vertex_PCUTBNSkinned(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords,
		                 Vec3 const& tangent, Vec3 const& bitangent, Vec3 const& normal,
						 Vec4 const& boneWeights, IntVec4 const& boneIndices);
// -----------------------------------------------------------------------------
	Vec3	m_position;
	Rgba8	m_color;
	Vec2	m_uvTexCoords;
	Vec3	m_tangent;
	Vec3	m_bitangent;
	Vec3	m_normal;
	Vec4	m_boneWeights;
	IntVec4 m_boneIndices;
};
// -----------------------------------------------------------------------------