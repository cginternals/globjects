#pragma once

#include <glow/glow.h>

#include <unordered_map>
#include <string>

namespace glow {

class GLOW_API ObjectProperties
{
public:
	unsigned int get(const std::string& property) const;

	void set(const std::string& property, unsigned int value);
	void clear(const std::string& property);

	void increase(const std::string& property);
	void decrease(const std::string& property);

	std::string getString(const std::string& property) const;
	void setString(const std::string& property, const std::string& value);
	void clearString(const std::string& property);

	const std::unordered_map<std::string, unsigned int>& properties() const;
	const std::unordered_map<std::string, std::string>& stringProperties() const;
public:
	void setMemory(unsigned int bytes);
	unsigned int memory() const;
protected:
	std::unordered_map<std::string, unsigned int> m_properties;
	std::unordered_map<std::string, std::string> m_stringProperties;
};

} // namespace glow
