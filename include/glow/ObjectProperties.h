#pragma once

#include <glow/glow.h>

#include <unordered_map>
#include <string>

namespace glow {

class GLOW_API ObjectProperties
{
public:
	unsigned int get(const std::string& property) const;

	void trackMemory(unsigned int bytes);
	void untrackMemory(unsigned int bytes);

	void set(const std::string& property, unsigned int value);
	void clear(const std::string& property);

	void increase(const std::string& property);
	void decrease(const std::string& property);

	const std::unordered_map<std::string, unsigned int>& properties() const;
protected:
	std::unordered_map<std::string, unsigned int> m_properties;
};

} // namespace glow
