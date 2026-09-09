#include "Engine/Core/NamedProperties.hpp"
#include "Engine/Core/HashedCaseInsensitiveString.hpp"

NamedProperties::NamedProperties(NamedProperties const& copyFrom)
{
	for (auto const& keyValuePair : copyFrom.m_keyValuePairs)
	{
		m_keyValuePairs[keyValuePair.first] = keyValuePair.second->Copy();
	}
}

NamedProperties& NamedProperties::operator=(NamedProperties const& copy)
{
	if (this == &copy)
	{
		return *this;
	}

	for (auto& keyValuePair : m_keyValuePairs)
	{
		delete keyValuePair.second;
	}
	m_keyValuePairs.clear();

	for (auto const& keyValuePair : copy.m_keyValuePairs)
	{
		m_keyValuePairs[keyValuePair.first] = keyValuePair.second->Copy();
	}

	return *this;
}

NamedProperties::~NamedProperties()
{
	for (auto& keyValuePair : m_keyValuePairs)
	{
		delete keyValuePair.second;
	}
	m_keyValuePairs.clear();
}

void NamedProperties::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	XmlAttribute const* attribute = element.FirstAttribute();
	while (attribute)
	{
		SetValue(attribute->Name(), attribute->Value());
		attribute = attribute->Next();
	}
}

void NamedProperties::SetValue(std::string const& keyName, std::string const& newValue)
{
	SetValue<std::string>(keyName, newValue);
}

void NamedProperties::SetValue(std::string const& keyName, char const* value)
{
	SetValue<std::string>(keyName, std::string(value));
}

std::string NamedProperties::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	return GetValue<std::string>(keyName, defaultValue);
}

std::string NamedProperties::GetValue(std::string const& keyName, char const* defaultValue) const
{
	return GetValue<std::string>(keyName, std::string(defaultValue));
}

bool NamedProperties::GetValue(std::string const& keyName, bool defaultValue) const
{
	return GetValue<bool>(keyName, defaultValue);
}

int NamedProperties::GetValue(std::string const& keyName, int defaultValue) const
{
	return GetValue<int>(keyName, defaultValue);
}

float NamedProperties::GetValue(std::string const& keyName, float defaultValue) const
{
	return GetValue<float>(keyName, defaultValue);
}

Rgba8 NamedProperties::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	return GetValue<Rgba8>(keyName, defaultValue);
}

Vec2 NamedProperties::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	return GetValue<Vec2>(keyName, defaultValue);
}

IntVec2 NamedProperties::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	return GetValue<IntVec2>(keyName, defaultValue);
}

void NamedProperties::GetAllKeyValuePairsAsStrings(std::map<std::string, std::string>& outMap) const
{
	for (auto const& pair : m_keyValuePairs)
	{
		std::string key = pair.first.GetOriginalString();

		NamedPropertyBase* base = pair.second;

		if (auto stringProp = dynamic_cast<NamedProperty<std::string>*>(base))
		{
			outMap[key] = stringProp->m_value;
		}
		else if (auto intProp = dynamic_cast<NamedProperty<int>*>(base))
		{
			outMap[key] = std::to_string(intProp->m_value);
		}
		else if (auto floatProp = dynamic_cast<NamedProperty<float>*>(base))
		{
			outMap[key] = std::to_string(floatProp->m_value);
		}
		else if (auto boolProp = dynamic_cast<NamedProperty<bool>*>(base))
		{
			outMap[key] = boolProp->m_value ? "true" : "false";
		}
	}
}


