#include "Engine/Core/HashedCaseInsensitiveString.hpp"

HashedCaseInsensitiveString::HashedCaseInsensitiveString(char const* text)
	:m_caseIntactText(text),
	 m_lowerCaseHash(CalculateHashFromText(text))
{
}

HashedCaseInsensitiveString::HashedCaseInsensitiveString(std::string text)
	:HashedCaseInsensitiveString(text.c_str())
{
}

unsigned int HashedCaseInsensitiveString::GetHash() const
{
	return m_lowerCaseHash;
}

std::string const& HashedCaseInsensitiveString::GetOriginalString() const
{
	return m_caseIntactText;
}

char const* HashedCaseInsensitiveString::c_str() const
{
	return m_caseIntactText.c_str();
}

bool HashedCaseInsensitiveString::operator<(HashedCaseInsensitiveString const& compare) const
{
	if (m_lowerCaseHash < compare.m_lowerCaseHash)
	{
		return true;
	}

	if (m_lowerCaseHash > compare.m_lowerCaseHash)
	{
		return false;
	}

	return _stricmp(m_caseIntactText.c_str(), compare.m_caseIntactText.c_str()) < 0;
}

bool HashedCaseInsensitiveString::operator==(HashedCaseInsensitiveString const& compare) const
{
	if (m_lowerCaseHash != compare.m_lowerCaseHash)
	{
		return false;
	}

	return _stricmp(m_caseIntactText.c_str(), compare.m_caseIntactText.c_str()) == 0;
}

bool HashedCaseInsensitiveString::operator!=(HashedCaseInsensitiveString const& compare) const
{
	return !(*this == compare);
}

bool HashedCaseInsensitiveString::operator==(char const* text) const
{
	unsigned int hash = CalculateHashFromText(text);

	if (m_lowerCaseHash != hash)
	{
		return false;
	}

	return _stricmp(m_caseIntactText.c_str(), text) == 0;
}

bool HashedCaseInsensitiveString::operator!=(char const* text) const
{
	return !(*this == text);
}

bool HashedCaseInsensitiveString::operator==(std::string const& text) const
{
	return (*this == text.c_str());
}

bool HashedCaseInsensitiveString::operator!=(std::string const& text) const
{
	return !(*this == text);
}

void HashedCaseInsensitiveString::operator=(HashedCaseInsensitiveString const& assignFrom)
{
	if (this == &assignFrom)
	{
		return;
	}

	m_caseIntactText = assignFrom.m_caseIntactText;
	m_lowerCaseHash = assignFrom.m_lowerCaseHash;
}

void HashedCaseInsensitiveString::operator=(char const* text)
{
	m_caseIntactText = text;
	m_lowerCaseHash = CalculateHashFromText(text);
}

void HashedCaseInsensitiveString::operator=(std::string const& text)
{
	m_caseIntactText = text;
	m_lowerCaseHash = CalculateHashFromText(text.c_str());
}

unsigned int HashedCaseInsensitiveString::CalculateHashFromText(char const* text)
{
	unsigned int hash = 0;
	for (char const* scan = text; *scan != '\0'; ++scan)
	{
		hash *= 31;
		hash += static_cast<unsigned int>(tolower(*scan));
	}
	return hash;
}
