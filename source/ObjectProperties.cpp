#include <glow/ObjectProperties.h>

namespace glow {

const std::unordered_map<std::string, unsigned int>& ObjectProperties::properties() const
{
	return m_properties;
}

const std::unordered_map<std::string, std::string>& ObjectProperties::stringProperties() const
{
	return m_stringProperties;
}

unsigned int ObjectProperties::get(const std::string& property) const
{
	return m_properties.count(property)>0 ? m_properties.at(property) : 0;
}

void ObjectProperties::setMemory(unsigned int bytes)
{
	set("_memory", bytes);
}

unsigned int ObjectProperties::memory() const
{
	return get("_memory");
}

void ObjectProperties::set(const std::string& property, unsigned int value)
{
	m_properties[property] = value;
}

void ObjectProperties::clear(const std::string& property)
{
	m_properties.erase(property);
}

void ObjectProperties::increase(const std::string& property)
{
	m_properties[property]++;
}

void ObjectProperties::decrease(const std::string& property)
{
	m_properties[property]--;
}

std::string ObjectProperties::getString(const std::string& property) const
{
	return m_stringProperties.count(property)>0 ? m_stringProperties.at(property) : "";
}

void ObjectProperties::setString(const std::string& property, const std::string& value)
{
	m_stringProperties[property] = value;
}

void ObjectProperties::clearString(const std::string& property)
{
	m_stringProperties.erase(property);
}

} // namespace glow
