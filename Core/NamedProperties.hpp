#pragma once
#include "Engine/Core/EngineCommon.h"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/HashedCaseInsensitiveString.hpp"
#include <map>
#include <string>
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class NamedPropertyBase
{
public:
	NamedPropertyBase() = default;
	virtual ~NamedPropertyBase() = default;
	virtual  NamedPropertyBase* Copy() const = 0;
};
// -----------------------------------------------------------------------------
template <typename T>
class NamedProperty : public NamedPropertyBase
{
public:
	explicit NamedProperty(T const& value)
		:m_value(value)
	{
	}
	virtual ~NamedProperty() override {}
	virtual NamedPropertyBase* Copy() const override
	{
		return new NamedProperty<T>(m_value);
	}

public:
	T m_value;
};
// -----------------------------------------------------------------------------
class NamedProperties
{
public:
	// Construction
	NamedProperties() = default;
	NamedProperties(NamedProperties const& copyFrom);
	NamedProperties& operator=(NamedProperties const& copy);
	~NamedProperties();

	// Template SetValue
	template <typename T>
	void SetValue(std::string const& keyName, T const& value)
	{
		HashedCaseInsensitiveString key(keyName);
		auto mapIterator = m_keyValuePairs.find(key);

		if (mapIterator != m_keyValuePairs.end())
		{
			delete mapIterator->second;
			mapIterator->second = new NamedProperty<T>(value);
		}
		else
		{
			m_keyValuePairs.emplace(key, new NamedProperty<T>(value));
		}
	}

	// Template GetValue
	template <typename T>
	T GetValue(std::string const& keyName, T const& value) const 
	{
		HashedCaseInsensitiveString key(keyName);
		auto mapIterator = m_keyValuePairs.find(key);

		if (mapIterator == m_keyValuePairs.end())
		{
			return value;
		}

		NamedPropertyBase* baseProperty = mapIterator->second;

		// Here we have a type match
		if (NamedProperty<T>* typedProperty = dynamic_cast<NamedProperty<T>*>(baseProperty))
		{
			return typedProperty->m_value;
		}

		// Convert if stored as string
		if (NamedProperty<std::string>* stringProperty = dynamic_cast<NamedProperty<std::string>*>(baseProperty))
		{
			return ConvertFromString(stringProperty->m_value, value);
		}
	
		return value;
	}

	// String Conversion
	template <typename T>
	T ConvertFromString(std::string const&, T const& defaultValue) const
	{
		return defaultValue;
	}

	// NamedStrings methods/backwards compatability
	void		PopulateFromXmlElementAttributes(XmlElement const& element);
	void        SetValue(std::string const& keyName, std::string const& newValue);
	void        SetValue(std::string const& keyName, char const* value);
	std::string GetValue(std::string const& keyName, std::string const& defaultValue) const;
	std::string GetValue(std::string const& keyName, char const* defaultValue) const;
	bool        GetValue(std::string const& keyName, bool defaultValue) const;
	int         GetValue(std::string const& keyName, int defaultValue) const;
	float       GetValue(std::string const& keyName, float defaultValue) const;
	Rgba8       GetValue(std::string const& keyName, Rgba8 const& defaultValue) const;
	Vec2        GetValue(std::string const& keyName, Vec2 const& defaultValue) const;
	IntVec2     GetValue(std::string const& keyName, IntVec2 const& defaultValue) const;
	void        GetAllKeyValuePairsAsStrings(std::map<std::string, std::string>& outMap) const;

private:
	std::map<HashedCaseInsensitiveString, NamedPropertyBase*> m_keyValuePairs;
};
// -----------------------------------------------------------------------------
template <>
inline bool NamedProperties::ConvertFromString<bool>(std::string const& text, bool const& defaultValue) const
{
	if (_stricmp(text.c_str(), "false") == 0)
	{
		return false;
	}
	if (_stricmp(text.c_str(), "true") == 0)
	{
		return true;
	}

	if (text == "0")
	{
		return false;
	}
	if (text == "1")
	{
		return true;
	}

	return defaultValue;
}

template <>
inline int NamedProperties::ConvertFromString<int>(std::string const& text, int const& defaultValue) const
{
	UNUSED(defaultValue);
	return atoi(text.c_str());
}

template <>
inline float NamedProperties::ConvertFromString<float>(std::string const& text, float const& defaultValue) const
{
	UNUSED(defaultValue);
	return static_cast<float>(atof(text.c_str()));
}

template <>
inline Vec2 NamedProperties::ConvertFromString<Vec2>(std::string const& text, Vec2 const& defaultValue) const
{
	Vec2 vectorValue = defaultValue;
	vectorValue.SetFromText(text.c_str());
	return vectorValue;
}

template <>
inline Rgba8 NamedProperties::ConvertFromString<Rgba8>(std::string const& text, Rgba8 const& defaultValue) const
{
	Rgba8 colorValue = defaultValue;
	colorValue.SetFromText(text.c_str());
	return colorValue;
}

template <>
inline IntVec2 NamedProperties::ConvertFromString<IntVec2>(std::string const& text, IntVec2 const& defaultValue) const
{
	IntVec2 intVectorValue = defaultValue;
	intVectorValue.SetFromText(text.c_str());
	return intVectorValue;
}
// -----------------------------------------------------------------------------
