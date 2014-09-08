#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <globjects/Object.h>


namespace globjects
{

class Object;
class Sync;

class AbstractObjectNameImplementation
{
public:
    AbstractObjectNameImplementation();
    virtual ~AbstractObjectNameImplementation();

    static AbstractObjectNameImplementation * get(Object::NameImplementation impl = 
        Object::NameImplementation::Legacy);

    virtual std::string getLabel(const Object * object) const = 0;
    virtual std::string getLabel(const Sync * sync) const = 0;

    virtual bool hasLabel(const Object * object) const = 0;
    virtual bool hasLabel(const Sync * sync) const = 0;

    virtual void setLabel(const Object * object, const std::string & label) const = 0;
    virtual void setLabel(const Sync * sync, const std::string & label) const = 0;

protected:
    void * objectLabelState(const Object * object) const;
    void * objectLabelState(const Sync * sync) const;

    void setObjectLabelState(const Object * object, void * state) const;
    void setObjectLabelState(const Sync * sync, void * state) const;
};

} // namespace globjects
