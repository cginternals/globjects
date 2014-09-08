
#include "ObjectNameImplementation_Legacy.h"

#include <globjects/globjects.h>

namespace globjects {

ObjectNameImplementation_Legacy::ObjectNameImplementation_Legacy()
{
}

ObjectNameImplementation_Legacy::~ObjectNameImplementation_Legacy()
{
}

std::string ObjectNameImplementation_Legacy::getLabel(const Object * object) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(object));

    if (state == nullptr)
    {
        return std::string();
    }

    return *state;
}

std::string ObjectNameImplementation_Legacy::getLabel(const Sync * sync) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(sync));

    if (state == nullptr)
    {
        return std::string();
    }

    return *state;
}

bool ObjectNameImplementation_Legacy::hasLabel(const Object * object) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(object));

    if (state == nullptr)
    {
        return false;
    }

    return state->size() > 0;
}

bool ObjectNameImplementation_Legacy::hasLabel(const Sync * sync) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(sync));

    if (state == nullptr)
    {
        return false;
    }

    return state->size() > 0;
}

void ObjectNameImplementation_Legacy::setLabel(const Object * object, const std::string & label) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(object));

    if (state == nullptr)
    {
        return setObjectLabelState(object, new std::string(label));
    }

    *state = label;
}

void ObjectNameImplementation_Legacy::setLabel(const Sync * sync, const std::string & label) const
{
    std::string * state = reinterpret_cast<std::string *>(objectLabelState(sync));

    if (state == nullptr)
    {
        return setObjectLabelState(sync, new std::string(label));
    }

    *state = label;
}

} // namespace globjects
