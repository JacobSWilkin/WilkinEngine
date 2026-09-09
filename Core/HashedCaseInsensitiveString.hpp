#pragma once
#include <string>
// -----------------------------------------------------------------------------
class HashedCaseInsensitiveString
{
public:
	// Construction
	HashedCaseInsensitiveString() = default;
	HashedCaseInsensitiveString(HashedCaseInsensitiveString const& copyFrom) = default;
	HashedCaseInsensitiveString(char const* text);
	HashedCaseInsensitiveString(std::string text);

	// Accessors
	unsigned int GetHash() const;
	std::string const& GetOriginalString() const;
	char const* c_str() const;

	// Operators
	bool operator<(HashedCaseInsensitiveString const& compare) const;
	bool operator==(HashedCaseInsensitiveString const& compare) const;
	bool operator!=(HashedCaseInsensitiveString const& compare) const;
	bool operator==(char const* text) const;
	bool operator!=(char const* text) const;
	bool operator==(std::string const& text) const;
	bool operator!=(std::string const& text) const;
	void operator=(HashedCaseInsensitiveString const& assignFrom);
	void operator=(char const* text);
	void operator=(std::string const& text);

	// Hash Calculation
	static unsigned int CalculateHashFromText(char const* text);

private:
	std::string  m_caseIntactText = "";
	unsigned int m_lowerCaseHash = 0;
};