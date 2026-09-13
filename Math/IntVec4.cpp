#include "Engine/Math/IntVec4.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <cmath>
// -----------------------------------------------------------------------------
IntVec4 const IntVec4::INVALID(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
IntVec4 const IntVec4::ZERO(0, 0, 0, 0);
IntVec4 const IntVec4::ONE(1, 1, 1, 1);
// -----------------------------------------------------------------------------
IntVec4::IntVec4(IntVec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

IntVec4::IntVec4(int initialX, int initialY, int initialZ, int initialW)
	:x(initialX),
	 y(initialY),
	 z(initialZ),
	 w(initialW)
{
}

float IntVec4::GetLength() const
{
	return (sqrtf(static_cast<float>(x * x) + 
		          static_cast<float>(y * y) + 
		          static_cast<float>(z * z) +
	              static_cast<float>(w * w)));
}

int IntVec4::GetTaxicabLength() const
{
	int dx = abs(x);
	int dy = abs(y);
	int dz = abs(z);
	int dw = abs(w);
	return dx + dy + dz + dw;
}

int IntVec4::GetLengthSquared() const
{
	return static_cast<int>(x * x) + 
		   static_cast<int>(y * y) + 
		   static_cast<int>(z * z) +
		   static_cast<int>(w * w);
}

void IntVec4::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	x = static_cast<int>(atoi(commaSplit[0].c_str()));
	y = static_cast<int>(atoi(commaSplit[1].c_str()));
	z = static_cast<int>(atoi(commaSplit[2].c_str()));
	w = static_cast<int>(atoi(commaSplit[3].c_str()));
}

bool IntVec4::operator==(IntVec4 const& compare) const
{
	return x == compare.x && y == compare.y && z == compare.z && w == compare.w;
}

bool IntVec4::operator!=(IntVec4 const& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z || w != compare.w;
}

IntVec4 const IntVec4::operator+(IntVec4 const& vecToAdd) const
{
	return IntVec4(vecToAdd.x + x, vecToAdd.y + y, vecToAdd.z + z, vecToAdd.w + w);
}

IntVec4 const IntVec4::operator-(IntVec4 const& vecToSubtract) const
{
	return IntVec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

IntVec4 const IntVec4::operator-() const
{
	return IntVec4(-(x), -(y), -(z), -(w));
}

IntVec4 const IntVec4::operator*(float uniformScale) const
{
	return IntVec4(x * static_cast<int>(uniformScale), 
		           y * static_cast<int>(uniformScale), 
		           z * static_cast<int>(uniformScale),
		           w * static_cast<int>(uniformScale));
}

IntVec4 const IntVec4::operator*(IntVec4 const& vecToMultiply) const
{
	return IntVec4(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z, w * vecToMultiply.w);
}

IntVec4 const IntVec4::operator/(float inverseScale) const
{
	return IntVec4(x / static_cast<int>(inverseScale), 
		           y / static_cast<int>(inverseScale), 
		           z / static_cast<int>(inverseScale),
		           w / static_cast<int>(inverseScale));
}

void IntVec4::operator+=(IntVec4 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

void IntVec4::operator-=(IntVec4 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

void IntVec4::operator*=(float const uniformScale)
{
	x *= static_cast<int>(uniformScale);
	y *= static_cast<int>(uniformScale);
	z *= static_cast<int>(uniformScale);
	w *= static_cast<int>(uniformScale);
}

void IntVec4::operator/=(float const uniformDivisor)
{
	x /= static_cast<int>(uniformDivisor);
	y /= static_cast<int>(uniformDivisor);
	z /= static_cast<int>(uniformDivisor);
	w /= static_cast<int>(uniformDivisor);
}

void IntVec4::operator=(IntVec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}
