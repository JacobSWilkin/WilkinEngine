#include "Engine/Core/BufferUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Vertex_PCU.h"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.h"
#include "Engine/Math/IntVec3.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Quat.hpp"
// -----------------------------------------------------------------------------
BufferParser::BufferParser(Buffer const& bufferToParse)
	: BufferParser(bufferToParse.data(), bufferToParse.size())
{
}

BufferParser::BufferParser(unsigned char const* dataStart, size_t sizeInBytes)
	:m_dataStart(dataStart),
	 m_currentReadPosition(dataStart),
	 m_sizeInBytes(sizeInBytes)
{
}

void BufferParser::SetEndianMode(EndianMode newEndianMode)
{
	m_endianMode = newEndianMode;

	if (m_endianMode != GetPlatformNativeEndianMode())
	{
		m_isOppositiveNativeEndianMode = true;
	}
	else
	{
		m_isOppositiveNativeEndianMode = false;
	}
}

EndianMode BufferParser::GetEndianMode() const
{
	return m_endianMode;
}

unsigned char BufferParser::ParseByte()
{
	CheckIfWeCanRead(1);

	unsigned char value = *m_currentReadPosition;
	++m_currentReadPosition;
	return value;
}

char BufferParser::ParseChar()
{
	return static_cast<char>(ParseByte());
}

bool BufferParser::ParseBool()
{
	unsigned char value = ParseByte();
	GUARANTEE_OR_DIE(value == 0 || value == 1, "Invalid boolean value in buffer, must be 0 or 1!");
	return value == 1;
}

uint16_t BufferParser::ParseUint16()
{
	CheckIfWeCanRead(sizeof(uint16_t));

	uint16_t value = *reinterpret_cast<uint16_t const*>(m_currentReadPosition);
	m_currentReadPosition += sizeof(uint16_t);

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap16(value);
	}

	return value;
}

uint32_t BufferParser::ParseUint32()
{
	CheckIfWeCanRead(sizeof(uint32_t));

	uint32_t value = *reinterpret_cast<uint32_t const*>(m_currentReadPosition);
	m_currentReadPosition += sizeof(uint32_t);

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap32(value);
	}

	return value;
}

uint64_t BufferParser::ParseUint64()
{
	CheckIfWeCanRead(sizeof(uint64_t));

	uint64_t value = *reinterpret_cast<uint64_t const*>(m_currentReadPosition);
	m_currentReadPosition += sizeof(uint64_t);

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap64(value);
	}

	return value;
}

int32_t BufferParser::ParseInt32()
{
	return static_cast<int32_t>(ParseUint32());
}

float BufferParser::ParseFloat()
{
	CheckIfWeCanRead(sizeof(float));

	float value = *reinterpret_cast<float const*>(m_currentReadPosition);
	m_currentReadPosition += sizeof(float);

	if (m_isOppositiveNativeEndianMode)
	{
		uint32_t* asInt = reinterpret_cast<uint32_t*>(&value);
		*asInt = ByteSwap32(*asInt);
	}

	return value;
}

double BufferParser::ParseDouble()
{
	CheckIfWeCanRead(sizeof(double));

	double value = *reinterpret_cast<double const*>(m_currentReadPosition);
	m_currentReadPosition += sizeof(double);

	if (m_isOppositiveNativeEndianMode)
	{
		uint64_t* asInt = reinterpret_cast<uint64_t*>(&value);
		*asInt = ByteSwap64(*asInt);
	}

	return value;
}

void BufferParser::ParseStringZeroTerminated(std::string& returned_string)
{
	returned_string.clear();
	
	while (m_currentReadPosition < m_dataStart + m_sizeInBytes)
	{
		char charValue = ParseChar();
		if (charValue == 0)
		{
			break;
		}
		returned_string += charValue;
	}
}

void BufferParser::ParseStringAfter32BitLength(std::string& returned_string)
{
	uint32_t strLen = ParseUint32();
	CheckIfWeCanRead(strLen);

	returned_string.clear();

	for (int charIndex = 0; charIndex < static_cast<int>(strLen); ++charIndex)
	{
		returned_string += ParseChar();
	}
}

Rgba8 const BufferParser::ParseRgba8()
{
	Rgba8 rgba;
	rgba.r = ParseByte();
	rgba.g = ParseByte();
	rgba.b = ParseByte();
	rgba.a = ParseByte();
	return rgba;
}

Rgba8 const BufferParser::ParseRgb()
{
	Rgba8 rgb;
	rgb.r = ParseByte();
	rgb.g = ParseByte();
	rgb.b = ParseByte();
	rgb.a = 255;
	return rgb;
}

Vec2 const BufferParser::ParseVec2()
{
	Vec2 vector2;
	vector2.x = ParseFloat();
	vector2.y = ParseFloat();
	return vector2;
}

Vec3 const BufferParser::ParseVec3()
{
	Vec3 vector3;
	vector3.x = ParseFloat();
	vector3.y = ParseFloat();
	vector3.z = ParseFloat();
	return vector3;
}

Vec4 const BufferParser::ParseVec4()
{
	Vec4 vector4;
	vector4.x = ParseFloat();
	vector4.y = ParseFloat();
	vector4.z = ParseFloat();
	vector4.w = ParseFloat();
	return vector4;
}

IntVec2 const BufferParser::ParseIntVec2()
{
	IntVec2 intVector2;
	intVector2.x = ParseInt32();
	intVector2.y = ParseInt32();
	return intVector2;
}

IntVec3 const BufferParser::ParseIntVec3()
{
	IntVec3 intVector3;
	intVector3.x = ParseInt32();
	intVector3.y = ParseInt32();
	intVector3.z = ParseInt32();
	return intVector3;
}

Plane2 const BufferParser::ParsePlane2()
{
	Plane2 plane;
	plane.m_planeNormal = ParseVec2();
	plane.m_distance    = ParseFloat();
	return plane;
}

Plane3 const BufferParser::ParsePlane3()
{
	Plane3 plane;
	plane.m_normal   = ParseVec3();
	plane.m_distance = ParseFloat();
	return plane;
}

AABB2 const BufferParser::ParseAABB2()
{
	AABB2 aabb2;
	aabb2.m_mins = ParseVec2();
	aabb2.m_maxs = ParseVec2();
	return aabb2;
}

AABB3 const BufferParser::ParseAABB3()
{
	AABB3 aabb3;
	aabb3.m_mins = ParseVec3();
	aabb3.m_maxs = ParseVec3();
	return aabb3;
}

OBB2 const BufferParser::ParseOBB2()
{
	OBB2 obb2;
	obb2.m_center = ParseVec2();
	obb2.m_iBasisNormal = ParseVec2();
	obb2.m_halfDimensions = ParseVec2();
	return obb2;
}

OBB3 const BufferParser::ParseOBB3()
{
	OBB3 obb3;
	obb3.m_center = ParseVec3();
	obb3.m_iBasis = ParseVec3();
	obb3.m_jBasis = ParseVec3();
	obb3.m_kBasis = ParseVec3();
	obb3.m_halfDimensions = ParseVec3();
	return obb3;
}

EulerAngles const BufferParser::ParseEulerAngles()
{
	EulerAngles eulerAngles;
	eulerAngles.m_yawDegrees   = ParseFloat();
	eulerAngles.m_pitchDegrees = ParseFloat();
	eulerAngles.m_rollDegrees  = ParseFloat();
	return eulerAngles;
}

Quat const BufferParser::ParseQuaternion()
{
	Quat quaternion;
	quaternion.m_x = ParseFloat();
	quaternion.m_y = ParseFloat();
	quaternion.m_z = ParseFloat();
	quaternion.m_w = ParseFloat();
	return quaternion;
}

Vertex_PCU const BufferParser::ParseVertexPCU()
{
	Vertex_PCU vPCU;
	vPCU.m_position    = ParseVec3();
	vPCU.m_color       = ParseRgba8();
	vPCU.m_uvTexCoords = ParseVec2();
	return vPCU;
}

Vertex_PCUTBN const BufferParser::ParseVertexPCUTBN()
{
	Vertex_PCUTBN vPCUTBN;
	vPCUTBN.m_position	= ParseVec3();
	vPCUTBN.m_color		= ParseRgba8();
	vPCUTBN.m_uvTexCoords = ParseVec2();
	vPCUTBN.m_tangent		= ParseVec3();
	vPCUTBN.m_bitangent	= ParseVec3();
	vPCUTBN.m_normal		= ParseVec3();
	return vPCUTBN;
}

void BufferParser::CheckIfWeCanRead(size_t numBytes)
{
	GUARANTEE_OR_DIE(m_currentReadPosition + numBytes <= m_dataStart + m_sizeInBytes, "Attempted to read past end of buffer.");
}

void BufferParser::JumpReadPosition(size_t newOffset)
{
	GUARANTEE_OR_DIE(newOffset <= m_sizeInBytes, "Read position out of bounds!");
	m_currentReadPosition = m_dataStart + newOffset;
}

size_t BufferParser::GetReadOffset() const
{
	return static_cast<size_t>(m_currentReadPosition - m_dataStart);
}
// -----------------------------------------------------------------------------

BufferWriter::BufferWriter(Buffer& buffertoWrite)
	:m_buffer(buffertoWrite)
{
}

void BufferWriter::SetEndianMode(EndianMode newEndianMode)
{
	m_endianMode = newEndianMode;

	if (m_endianMode != GetPlatformNativeEndianMode())
	{
		m_isOppositiveNativeEndianMode = true;
	}
	else
	{
		m_isOppositiveNativeEndianMode = false;
	}
}

EndianMode BufferWriter::GetEndianMode() const
{
	return m_endianMode;
}

void BufferWriter::AppendByte(unsigned char byteToWrite)
{
	m_buffer.push_back(byteToWrite);
}

void BufferWriter::AppendBytes(void const* data, size_t byteCount)
{
	if (byteCount == 0)
	{
		return;
	}

	unsigned char const* bytes = reinterpret_cast<unsigned char const*>(data);
	m_buffer.insert(m_buffer.end(), bytes, bytes + byteCount);
}

void BufferWriter::AppendChar(char charToWrite)
{
	m_buffer.push_back(charToWrite);
}

void BufferWriter::AppendBool(bool boolToWrite)
{
	if (boolToWrite)
	{
		m_buffer.push_back(1);
	}
	else
	{
		m_buffer.push_back(0);
	}
}

void BufferWriter::AppendUint16(uint16_t uint16ToWrite)
{
	uint16_t value = uint16ToWrite;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap16(value);
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&value);

	m_buffer.push_back(bytePtr[0]);
	m_buffer.push_back(bytePtr[1]);
}

void BufferWriter::AppendUint32(uint32_t uint32ToWrite)
{
	uint32_t value = uint32ToWrite;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap32(value);
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&value);

	m_buffer.push_back(bytePtr[0]);
	m_buffer.push_back(bytePtr[1]);
	m_buffer.push_back(bytePtr[2]);
	m_buffer.push_back(bytePtr[3]);
}

void BufferWriter::AppendUint64(uint64_t uint64ToWrite)
{
	uint64_t value = uint64ToWrite;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap64(value);
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&value);

	m_buffer.push_back(bytePtr[0]);
	m_buffer.push_back(bytePtr[1]);
	m_buffer.push_back(bytePtr[2]);
	m_buffer.push_back(bytePtr[3]);
	m_buffer.push_back(bytePtr[4]);
	m_buffer.push_back(bytePtr[5]);
	m_buffer.push_back(bytePtr[6]);
	m_buffer.push_back(bytePtr[7]);
}

void BufferWriter::AppendInt32(int32_t int32ToWrite)
{
	AppendUint32(static_cast<uint32_t>(int32ToWrite));
}

void BufferWriter::AppendFloat(float floatToWrite)
{
	uint32_t* asUint = reinterpret_cast<uint32_t*>(&floatToWrite);
	uint32_t value = *asUint;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap32(value);
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&value);

	m_buffer.push_back(bytePtr[0]);
	m_buffer.push_back(bytePtr[1]);
	m_buffer.push_back(bytePtr[2]);
	m_buffer.push_back(bytePtr[3]);
}

void BufferWriter::AppendDouble(double doubleToWrite)
{
	uint64_t* asUint = reinterpret_cast<uint64_t*>(&doubleToWrite);
	uint64_t value = *asUint;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap64(value);
	}

	unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&value);

	m_buffer.push_back(bytePtr[0]);
	m_buffer.push_back(bytePtr[1]);
	m_buffer.push_back(bytePtr[2]);
	m_buffer.push_back(bytePtr[3]);
	m_buffer.push_back(bytePtr[4]);
	m_buffer.push_back(bytePtr[5]);
	m_buffer.push_back(bytePtr[6]);
	m_buffer.push_back(bytePtr[7]);
}

void BufferWriter::AppendStringZeroTerminated(std::string const& stringToWrite)
{
	for (int charIndex = 0; charIndex < static_cast<int>(stringToWrite.length()); ++charIndex)
	{
		AppendChar(stringToWrite[charIndex]);
	}
	AppendChar(0);
}

void BufferWriter::AppendStringAfter32BitLength(std::string const& stringToWrite)
{
	uint32_t value = static_cast<uint32_t>(stringToWrite.length());
	AppendUint32(value);

	for (int charIndex = 0; charIndex < static_cast<int>(stringToWrite.length()); ++charIndex)
	{
		AppendChar(stringToWrite[charIndex]);
	}
}

void BufferWriter::AppendRgba8(Rgba8 const& rgbaToWrite)
{
	AppendByte(rgbaToWrite.r);
	AppendByte(rgbaToWrite.g);
	AppendByte(rgbaToWrite.b);
	AppendByte(rgbaToWrite.a);
}

void BufferWriter::AppendRgb(Rgba8 const& rgbToWrite)
{
	AppendByte(rgbToWrite.r);
	AppendByte(rgbToWrite.g);
	AppendByte(rgbToWrite.b);
}

void BufferWriter::AppendVec2(Vec2 const& vec2ToWrite)
{
	AppendFloat(vec2ToWrite.x);
	AppendFloat(vec2ToWrite.y);
}

void BufferWriter::AppendVec3(Vec3 const& vec3ToWrite)
{
	AppendFloat(vec3ToWrite.x);
	AppendFloat(vec3ToWrite.y);
	AppendFloat(vec3ToWrite.z);
}

void BufferWriter::AppendVec4(Vec4 const& vec4ToWrite)
{
	AppendFloat(vec4ToWrite.x);
	AppendFloat(vec4ToWrite.y);
	AppendFloat(vec4ToWrite.z);
	AppendFloat(vec4ToWrite.w);
}

void BufferWriter::AppendIntVec2(IntVec2 const& intVec2ToWrite)
{
	AppendInt32(intVec2ToWrite.x);
	AppendInt32(intVec2ToWrite.y);
}

void BufferWriter::AppendIntVec3(IntVec3 const& intVec3ToWrite)
{
	AppendInt32(intVec3ToWrite.x);
	AppendInt32(intVec3ToWrite.y);
	AppendInt32(intVec3ToWrite.z);
}

void BufferWriter::AppendEulerAngles(EulerAngles const& eulerAnglesToWrite)
{
	AppendFloat(eulerAnglesToWrite.m_yawDegrees);
	AppendFloat(eulerAnglesToWrite.m_pitchDegrees);
	AppendFloat(eulerAnglesToWrite.m_rollDegrees);
}

void BufferWriter::AppendQuaternion(Quat const& quaternionToWrite)
{
	AppendFloat(quaternionToWrite.m_x);
	AppendFloat(quaternionToWrite.m_y);
	AppendFloat(quaternionToWrite.m_z);
	AppendFloat(quaternionToWrite.m_w);
}

void BufferWriter::AppendPlane2(Plane2 const& plane2ToWrite)
{
	AppendVec2(plane2ToWrite.m_planeNormal);
	AppendFloat(plane2ToWrite.m_distance);
}

void BufferWriter::AppendPlane3(Plane3 const& plane3ToWrite)
{
	AppendVec3(plane3ToWrite.m_normal);
	AppendFloat(plane3ToWrite.m_distance);
}

void BufferWriter::AppendAABB2(AABB2 const& aabb2ToWrite)
{
	AppendVec2(aabb2ToWrite.m_mins);
	AppendVec2(aabb2ToWrite.m_maxs);
}

void BufferWriter::AppendAABB3(AABB3 const& aabb3ToWrite)
{
	AppendVec3(aabb3ToWrite.m_mins);
	AppendVec3(aabb3ToWrite.m_maxs);
}

void BufferWriter::AppendOBB2(OBB2 const& obb2ToWrite)
{
	AppendVec2(obb2ToWrite.m_center);
	AppendVec2(obb2ToWrite.m_iBasisNormal);
	AppendVec2(obb2ToWrite.m_halfDimensions);
}

void BufferWriter::AppendOBB3(OBB3 const& obb3ToWrite)
{
	AppendVec3(obb3ToWrite.m_center);
	AppendVec3(obb3ToWrite.m_iBasis);
	AppendVec3(obb3ToWrite.m_jBasis);
	AppendVec3(obb3ToWrite.m_kBasis);
	AppendVec3(obb3ToWrite.m_halfDimensions);
}

void BufferWriter::AppendVertexPCU(Vertex_PCU const& vertexPCUToWrite)
{
	AppendVec3(vertexPCUToWrite.m_position);
	AppendRgba8(vertexPCUToWrite.m_color);
	AppendVec2(vertexPCUToWrite.m_uvTexCoords);
}

void BufferWriter::AppendVertexPCUTBN(Vertex_PCUTBN const& vertexPCUTBNToWrite)
{
	AppendVec3(vertexPCUTBNToWrite.m_position);
	AppendRgba8(vertexPCUTBNToWrite.m_color);
	AppendVec2(vertexPCUTBNToWrite.m_uvTexCoords);
	AppendVec3(vertexPCUTBNToWrite.m_tangent);
	AppendVec3(vertexPCUTBNToWrite.m_bitangent);
	AppendVec3(vertexPCUTBNToWrite.m_normal);
}

void BufferWriter::OverwriteUint32AtLocationInBuffer(uint32_t overwriteValue, size_t overwriteLocation)
{
	GUARANTEE_OR_DIE(overwriteLocation + sizeof(uint32_t) <= m_buffer.size(), "Overwrite location is out of bounds!");
	uint32_t value = overwriteValue;

	if (m_isOppositiveNativeEndianMode)
	{
		value = ByteSwap32(value);
	}

	uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);

	m_buffer[overwriteLocation + 0] = valueBytes[0];
	m_buffer[overwriteLocation + 1] = valueBytes[1];
	m_buffer[overwriteLocation + 2] = valueBytes[2];
	m_buffer[overwriteLocation + 3] = valueBytes[3];
}

// -----------------------------------------------------------------------------
EndianMode GetPlatformNativeEndianMode()
{
	uint32_t testValue = 0x12345678u;
	unsigned char* bytes = reinterpret_cast<unsigned char*>(&testValue);

	if (bytes[0] == 0x78)
	{
		return EndianMode::LITTLE;
	}
	else
	{
		return EndianMode::BIG;
	}
}

uint16_t ByteSwap16(uint16_t value)
{
	return (value >> 8) | (value << 8);
}

uint32_t ByteSwap32(uint32_t value)
{
	uint32_t val1 = (value & 0x000000FF) << 24;
	uint32_t val2 = (value & 0x0000FF00) << 8;
	uint32_t val3 = (value & 0x00FF0000) >> 8;
	uint32_t val4 = (value & 0xFF000000) >> 24;
	return val1 | val2 | val3 | val4;
}

uint64_t ByteSwap64(uint64_t value)
{
	uint64_t val1 = (value & 0x00000000000000FFULL) << 56;
	uint64_t val2 = (value & 0x000000000000FF00ULL) << 40;
	uint64_t val3 = (value & 0x0000000000FF0000ULL) << 24;
	uint64_t val4 = (value & 0x00000000FF000000ULL) << 8;
	uint64_t val5 = (value & 0x000000FF00000000ULL) >> 8;
	uint64_t val6 = (value & 0x0000FF0000000000ULL) >> 24;
	uint64_t val7 = (value & 0x00FF000000000000ULL) >> 40;
	uint64_t val8 = (value & 0xFF00000000000000ULL) >> 56;
	return val1 | val2 | val3 | val4 | val5 | val6 | val7 | val8;
}
