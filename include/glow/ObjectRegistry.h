#pragma once

#include <set>

#include <glow/glow.h>

namespace glow 
{

class Object;

/** \brief Tracks all wrapped OpenGL objects in glow.
    
    To obtain all wrapped objects use objects().
    The other methods are not meant to be called by the user.
*/
class GLOW_API ObjectRegistry
{
private:
	ObjectRegistry();

public:
	static const std::set<Object *> & objects();

	static void registerObject(Object * object);
	static void deregisterObject(Object * object);

protected:
	static std::set<Object *> s_objects;
};

} // namespace glow
