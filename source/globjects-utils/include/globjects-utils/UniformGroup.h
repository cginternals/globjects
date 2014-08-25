#pragma once

#include <unordered_map>
#include <string>

#include <globjects/AbstractUniform.h>
#include <globjects/Uniform.h>

#include <globjects-utils/globjects-utils_api.h>

namespace glo
{
    class AbstractUniform;
    class Program;
}

namespace gloutils
{

class GLOBJECTS_UTILS_API UniformGroup
{
public:
    UniformGroup();
    virtual ~UniformGroup();

    template <typename T>
    glo::Uniform<T> * uniform(const std::string & name);

    template <typename T>
    const glo::Uniform<T> * uniform(const std::string & name) const;

    void addUniform(glo::AbstractUniform * uniform);
    void addToProgram(glo::Program * program);

protected:
    std::unordered_map<std::string, glo::ref_ptr<glo::AbstractUniform>> m_uniforms;
};

} // namespace gloutils

#include <globjects-utils/UniformGroup.hpp>
