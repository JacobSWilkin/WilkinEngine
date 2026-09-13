#pragma once
// -----------------------------------------------------------------------------
struct IntVec4
{
	// static const declarations
	static const IntVec4 INVALID;
	static const IntVec4 ZERO;
	static const IntVec4 ONE;

public:
	int x = 0;
	int y = 0;
	int z = 0;
	int w = 0;

public:
	IntVec4() {}
	~IntVec4() {}
	IntVec4(IntVec4 const& copyFrom);
	explicit IntVec4(int initialX, int initialY, int initialZ, int initialW);

	// Accessors
	float GetLength() const;
	int   GetTaxicabLength() const;
	int   GetLengthSquared() const;

	// Mutators
	void  SetFromText(char const* text);

	// Operators
	bool		    operator==(IntVec4 const& compare) const;
	bool		    operator!=(IntVec4 const& compare) const;
	IntVec4 const	operator+(IntVec4 const& vecToAdd) const;
	IntVec4 const	operator-(IntVec4 const& vecToSubtract) const;
	IntVec4 const	operator-() const;
	IntVec4 const	operator*(float uniformScale) const;
	IntVec4 const	operator*(IntVec4 const& vecToMultiply) const;
	IntVec4 const	operator/(float inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(IntVec4 const& vecToAdd);
	void		operator-=(IntVec4 const& vecToSubtract);
	void		operator*=(float const uniformScale);
	void		operator/=(float const uniformDivisor);
	void		operator=(IntVec4 const& copyFrom);
};