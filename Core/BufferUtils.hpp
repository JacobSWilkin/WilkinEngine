#pragma once
#include <string>
#include <vector>
// -----------------------------------------------------------------------------
struct Rgba8;
struct Vertex_PCU;
struct Vertex_PCUTBN;
struct Vec2;
struct Vec3;
struct Vec4;
struct IntVec2;
struct IntVec3;
struct Plane2;
struct Plane3;
struct AABB2;
struct AABB3;
struct OBB2;
struct OBB3;
struct EulerAngles;
class  Quat;
// -----------------------------------------------------------------------------
typedef std::vector<uint8_t> Buffer;
// -----------------------------------------------------------------------------
enum class EndianMode
{
	NATIVE,
	LITTLE,
	BIG
};
// -----------------------------------------------------------------------------
// Helpers
EndianMode GetPlatformNativeEndianMode();
uint16_t   ByteSwap16(uint16_t value);
uint32_t   ByteSwap32(uint32_t value);
uint64_t   ByteSwap64(uint64_t value);
// -----------------------------------------------------------------------------
struct BufferWriter
{
public:
	BufferWriter(Buffer& buffertoWrite);

	// Endianness
	void       SetEndianMode(EndianMode newEndianMode);
	EndianMode GetEndianMode() const;

	// Writing primitives singular
	void AppendByte(unsigned char byteToWrite);
	void AppendBytes(void const* data, size_t byteCount);
	void AppendChar(char charToWrite);
	void AppendBool(bool boolToWrite);

	// Writing primitives multi
	void AppendUint16(uint16_t uint16ToWrite);
	void AppendUint32(uint32_t uint32ToWrite);
	void AppendUint64(uint64_t uint64ToWrite);
	void AppendInt32(int32_t int32ToWrite);
	void AppendFloat(float floatToWrite);
	void AppendDouble(double doubleToWrite);
	void AppendStringZeroTerminated(std::string const& stringToWrite);
	void AppendStringAfter32BitLength(std::string const& stringToWrite);

	// Writing semi-primitives
	void AppendRgba8(Rgba8 const& rgbaToWrite);
	void AppendRgb(Rgba8 const& rgbToWrite);
	void AppendVec2(Vec2 const& vec2ToWrite);
	void AppendVec3(Vec3 const& vec3ToWrite);
	void AppendVec4(Vec4 const& vec4ToWrite);
	void AppendIntVec2(IntVec2 const& intVec2ToWrite);
	void AppendIntVec3(IntVec3 const& intVec3ToWrite);
	void AppendEulerAngles(EulerAngles const& eulerAnglesToWrite);
	void AppendQuaternion(Quat const& quaternionToWrite);
	void AppendPlane2(Plane2 const& plane2ToWrite);
	void AppendPlane3(Plane3 const& plane3ToWrite);
	void AppendAABB2(AABB2 const& aabb2ToWrite);
	void AppendAABB3(AABB3 const& aabb3ToWrite);
	void AppendOBB2(OBB2 const& obb2ToWrite);
	void AppendOBB3(OBB3 const& obb3ToWrite);
	void AppendVertexPCU(Vertex_PCU const& vertexPCUToWrite);
	void AppendVertexPCUTBN(Vertex_PCUTBN const& vertexPCUTBNToWrite);

	// Location overwrite
	void OverwriteUint32AtLocationInBuffer(uint32_t overwriteValue, size_t overwriteLocation);

public:
	Buffer&    m_buffer;
	EndianMode m_endianMode = EndianMode::NATIVE;
	bool       m_isOppositiveNativeEndianMode = false;
};
// -----------------------------------------------------------------------------
struct BufferParser
{
public:
	BufferParser(Buffer const& bufferToParse);
	BufferParser(unsigned char const* dataStart, size_t sizeInBytes);

	// Endianness
	void       SetEndianMode(EndianMode newEndianMode);
	EndianMode GetEndianMode() const;

	// Parsing primitives singular
	unsigned char ParseByte();
	char          ParseChar();
	bool          ParseBool();

	// Parsing primitives multi
	uint16_t      ParseUint16();
	uint32_t      ParseUint32();
	uint64_t      ParseUint64();
	int32_t       ParseInt32();
	float         ParseFloat();
	double        ParseDouble();
	void          ParseStringZeroTerminated(std::string& returned_string);
	void		  ParseStringAfter32BitLength(std::string& returned_string);

	// Parsing uniques (semi-primitive)
	Rgba8		  const ParseRgba8();
	Rgba8		  const ParseRgb();
	Vec2		  const ParseVec2();
	Vec3		  const ParseVec3();
	Vec4		  const ParseVec4();
	IntVec2		  const ParseIntVec2();
	IntVec3		  const ParseIntVec3();
	Plane2        const ParsePlane2();
	Plane3        const ParsePlane3();
	AABB2         const ParseAABB2();
	AABB3         const ParseAABB3();
	OBB2          const ParseOBB2();
	OBB3          const ParseOBB3();
	EulerAngles   const ParseEulerAngles();
	Quat          const ParseQuaternion();
	Vertex_PCU	  const ParseVertexPCU();
	Vertex_PCUTBN const ParseVertexPCUTBN();

	// Helpers
	void CheckIfWeCanRead(size_t numBytes);
	void JumpReadPosition(size_t newOffset);
	size_t GetReadOffset() const;

public:
	unsigned char const* m_dataStart = nullptr;
	unsigned char const* m_currentReadPosition = nullptr;
	size_t               m_sizeInBytes = 0;
	EndianMode           m_endianMode = EndianMode::NATIVE;
	bool                 m_isOppositiveNativeEndianMode = false;
};
// -----------------------------------------------------------------------------