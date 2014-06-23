#pragma once

#include <set>

#include <glow/glow_api.h>

namespace glow 
{

class Object;
class FrameBufferObject;
class VertexArrayObject;

/** \brief Tracks all wrapped OpenGL objects in glow.
    
    To obtain all wrapped objects use objects().
*/
class GLOW_API ObjectRegistry
{
    friend class Object;
public:
	ObjectRegistry();
    static ObjectRegistry & current();

    const std::set<Object *> & objects();

    std::set<Object *>::iterator begin();
    std::set<Object *>::iterator end();

    FrameBufferObject * defaultFBO();
    VertexArrayObject * defaultVAO();
protected:
    void registerObject(Object * object);
    void deregisterObject(Object * object);

protected:
    std::set<Object *> m_objects;
    FrameBufferObject * m_defaultFBO;
    VertexArrayObject * m_defaultVAO;
};

} // namespace glow
