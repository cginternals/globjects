#include <glow/StateSetting.h>

#include <glow/Error.h>

namespace glow
{

StateSettingType::StateSettingType()
: m_functionIdentifier(nullptr)
{
}

StateSettingType::StateSettingType(void * functionIdentifier)
: m_functionIdentifier(functionIdentifier)
{
}

bool StateSettingType::operator==(const StateSettingType & other) const
{
    return m_functionIdentifier == other.m_functionIdentifier && m_subtypes == other.m_subtypes;
}

std::size_t StateSettingType::hash() const
{
    return std::hash<void*>()(m_functionIdentifier);
}

void StateSettingType::specializeType(GLenum subtype)
{
    m_subtypes.insert(subtype);
}

StateSetting::StateSetting(AbstractFunctionCall * functionCall)
: m_functionCall(functionCall)
, m_type(m_functionCall->identifier())
{
}

StateSetting::~StateSetting()
{
}

void StateSetting::apply()
{
    (*m_functionCall)();
    CheckGLError();
}

const StateSettingType & StateSetting::type() const
{
    return m_type;
}

StateSettingType & StateSetting::type()
{
    return m_type;
}

} // namespace glow

namespace std {

size_t hash<glow::StateSettingType>::operator()(const glow::StateSettingType & type) const
{
    return type.hash();
}

} // namespace std
