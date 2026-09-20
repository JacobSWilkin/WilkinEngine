#pragma once
// -----------------------------------------------------------------------------
struct Vec2;
// -----------------------------------------------------------------------------
// 2D Distance Shapes
float SDFCircle(Vec2 const& point, float radius);
float SDFAABB(Vec2 const& point, Vec2 const& halfDimensions);