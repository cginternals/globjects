
#include "ObjectNameImplementation_KHR_debug.h"

#include <cassert>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include <globjects/Object.h>
#include <globjects/Sync.h>

using namespace gl;

namespace globjects {

ObjectNameImplementation_KHR_debug::ObjectNameImplementation_KHR_debug()
    : m_maxLabelLength(getInteger(GL_MAX_LABEL_LENGTH))
{
}

ObjectNameImplementation_KHR_debug::~ObjectNameImplementation_KHR_debug()
{
}

std::string ObjectNameImplementation_KHR_debug::getLabel(const Object * object) const
{
    std::vector<char> data(m_maxLabelLength);
    GLsizei length = 0;

    glGetObjectLabel(object->objectType(), object->id(), m_maxLabelLength, &length, data.data());

    return std::string(data.data(), length);
}

std::string ObjectNameImplementation_KHR_debug::getLabel(const Sync * sync) const
{
    std::vector<char> data(m_maxLabelLength);
    GLsizei length = 0;

    glGetObjectPtrLabel(sync->sync(), m_maxLabelLength, &length, data.data());

    return std::string(data.data(), length);
}

bool ObjectNameImplementation_KHR_debug::hasLabel(const Object * object) const
{
    char character = 0;
    GLsizei length = 0;

    glGetObjectLabel(object->objectType(), object->id(), 1, &length, &character);

    return length > 0;
}

bool ObjectNameImplementation_KHR_debug::hasLabel(const Sync * sync) const
{
    char character = 0;
    GLsizei length = 0;

    glGetObjectPtrLabel(sync->sync(), 1, &length, &character);

    return length > 0;
}

void ObjectNameImplementation_KHR_debug::setLabel(const Object * object, const std::string & label) const
{
    assert(label.size() <= m_maxLabelLength);

    glObjectLabel(object->objectType(), object->id(), static_cast<GLsizei>(label.size()), label.data());
}

void ObjectNameImplementation_KHR_debug::setLabel(const Sync * sync, const std::string & label) const
{
    assert(label.size() <= m_maxLabelLength);

    glObjectPtrLabel(sync->sync(), static_cast<GLsizei>(label.size()), label.data());
}

} // namespace globjects
