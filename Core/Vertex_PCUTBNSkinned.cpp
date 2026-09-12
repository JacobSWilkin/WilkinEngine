#include "Engine/Core/Vertex_PCUTBNSkinned.hpp"

Vertex_PCUTBNSkinned::Vertex_PCUTBNSkinned(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords, Vec3 const& tangent, Vec3 const& bitangent, Vec3 const& normal, Vec4 const& boneWeights, IntVec4 const& boneIndices)
	:m_position(position),
	 m_color(color),
	 m_uvTexCoords(uvTexCoords),
	 m_tangent(tangent),
	 m_bitangent(bitangent),
	 m_normal(normal),
	 m_boneWeights(boneWeights),
	 m_boneIndices(boneIndices)
{
}
