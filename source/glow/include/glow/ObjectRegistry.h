#pragma once

#include <set>

#include <glow/glow_api.h>

namespace glow 
{

class Object;

/** \brief Tracks all wrapped OpenGL objects in glow.
    
    To obtain all wrapped objects use objects().
*/
class GLOW_API ObjectRegistry
{
    friend class Object;
public:
	ObjectRegistry();

    const std::set<Object *> & objects();

protected:
    void registerObject(Object * object);
    void deregisterObject(Object * object);

protected:
    std::set<Object *> m_objects;
};

} // namespace glow
