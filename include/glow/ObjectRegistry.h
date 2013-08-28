#pragma once

#include <set>
#include <string>

#include <glow/Object.h>

namespace glow {

class GLOW_API ObjectRegistry
{
private:
	ObjectRegistry();
public:
	static ObjectRegistry& instance();

	void registerObject(Object* object);
	void deregisterObject(Object* object);

	void dumpInfo() const;

	std::string info() const;
protected:
	std::set<Object*> m_objects;

	std::string humanReadableSize(unsigned bytes) const;
};

} // namespace glow
