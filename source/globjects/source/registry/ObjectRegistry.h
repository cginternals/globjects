#pragma once

#include <set>
#include <memory>

namespace globjects 
{

class Object;
class Framebuffer;
class VertexArray;

/** \brief Tracks all wrapped OpenGL objects in globjects.
    
    To obtain all wrapped objects use objects().
*/
class ObjectRegistry
{
    friend class Object;
public:
    ObjectRegistry();
    virtual ~ObjectRegistry() = default;

    static ObjectRegistry & current();

    std::set<Object *> objects() const;

    std::set<Object *>::iterator begin();
    std::set<Object *>::iterator end();

    Framebuffer * defaultFBO();
    VertexArray * defaultVAO();

protected:
    void registerObject(Object * object);
    void deregisterObject(Object * object);

protected:
    std::set<Object *> m_objects;
    std::unique_ptr<Framebuffer> m_defaultFBO;
    std::unique_ptr<VertexArray> m_defaultVAO;
};

} // namespace globjects
