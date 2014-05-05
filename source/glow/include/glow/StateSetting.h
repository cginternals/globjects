#pragma once

#include <utility>
#include <set>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glowbase/FunctionCall.h>

namespace glow
{

class GLOW_API StateSettingType
{
public:
    StateSettingType();
    StateSettingType(void * functionIdentifier);

    bool operator==(const StateSettingType & other) const;
    std::size_t hash() const;

    void specializeType(GLenum subtype);
protected:
    void * m_functionIdentifier;
    std::set<GLenum> m_subtypes;
};

class GLOW_API StateSetting
{
public:
    StateSetting(AbstractFunctionCall * functionCall);
    template <typename... Arguments>
    StateSetting(void (*function)(Arguments...), Arguments... arguments);

    virtual ~StateSetting();

    void apply();

    StateSettingType & type();
    const StateSettingType & type() const;
protected:
    AbstractFunctionCall * m_functionCall;
    StateSettingType m_type;
};

} // namespace glow

namespace std {

template <>
struct GLOW_API hash<glow::StateSettingType>
{
    size_t operator()(const glow::StateSettingType & type) const;
};

} // namespace glow

#include <glow/StateSetting.hpp>
