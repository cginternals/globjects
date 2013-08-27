#include <glow/ObjectProperties.h>

namespace glow {

const std::unordered_map<std::string, unsigned int>& ObjectProperties::properties() const
{
	return m_properties;
}

unsigned int ObjectProperties::get(const std::string& property) const
{
	return m_properties.count(property)>0 ? m_properties.at(property) : 0;
}

void ObjectProperties::trackMemory(unsigned int bytes)
{
	m_properties["_memory"] += bytes;
}

void ObjectProperties::untrackMemory(unsigned int bytes)
{
	m_properties["_memory"] -= bytes;
}

void ObjectProperties::set(const std::string& property, unsigned int value)
{
	m_properties[property] = value;
}

void ObjectProperties::clear(const std::string& property)
{
	m_properties[property] = 0;
}

void ObjectProperties::increase(const std::string& property)
{
	m_properties[property]++;
}

void ObjectProperties::decrease(const std::string& property)
{
	m_properties[property]--;
}

} // namespace glow
