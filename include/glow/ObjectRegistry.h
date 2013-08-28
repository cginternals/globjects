#pragma once

#include <set>
#include <string>
#include <vector>

#include <glow/Object.h>

namespace glow {

class GLOW_API ObjectRegistry
{
private:
	ObjectRegistry();
public:
	struct KeyValuePair { std::string key; std::string value; };
	struct ObjectInfo { std::string name; std::vector<KeyValuePair> properties; };
	struct ObjectInfoGroup { std::string name; std::vector<ObjectInfo> objects; };

	static ObjectRegistry& instance();

	void registerObject(Object* object);
	void deregisterObject(Object* object);

	void dumpInfo() const;

	std::string info() const;

	std::vector<ObjectInfoGroup> infoGroups() const;
protected:
	std::set<Object*> m_objects;

	std::string name(Object* object) const;
	std::string humanReadableSize(unsigned bytes) const;
};

} // namespace glow
