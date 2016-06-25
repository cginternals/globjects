
#pragma once

#include <functional>
#include <set>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects
{


class AbstractFunctionCall;

class GLOBJECTS_API StateSettingType
{
public:
    StateSettingType();
    StateSettingType(void * functionIdentifier);

    bool operator==(const StateSettingType & other) const;
    std::size_t hash() const;

    void specializeType(gl::GLenum subtype);

protected:
    void * m_functionIdentifier;
    std::set<gl::GLenum> m_subtypes;
};


class GLOBJECTS_API StateSetting
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

} // namespace globjects


namespace std 
{

template <>
struct GLOBJECTS_API hash<globjects::StateSettingType>
{
    size_t operator()(const globjects::StateSettingType & type) const;
};

} // namespace globjects


#include <globjects/StateSetting.inl>
