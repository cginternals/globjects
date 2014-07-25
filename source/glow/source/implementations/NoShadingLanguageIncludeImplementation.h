#pragma once

#include "AbstractShadingLanguageIncludeImplementation.h"

namespace glow
{

class NoShadingLanguageIncludeImplementation : public AbstractShadingLanguageIncludeImplementation
{
public:
    virtual std::vector<const char*> getSources(const Shader* shader) const override;
    virtual void compile(const Shader* shader) const override;
};

} // namespace glow
