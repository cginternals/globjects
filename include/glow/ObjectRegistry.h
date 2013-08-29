#pragma once

#include <set>

#include <glow/Object.h>

namespace glow {

class GLOW_API ObjectRegistry
{
private:
	ObjectRegistry();
public:
	static const std::set<Object*>& objects();

	static void registerObject(Object* object);
	static void deregisterObject(Object* object);
protected:
	static std::set<Object*> s_objects;
};

} // namespace glow
