#pragma once

#include "AbstractShadingLanguageIncludeImplementation.h"

namespace glow
{

class ShadingLanguageIncludeImplementation : public AbstractShadingLanguageIncludeImplementation
{
public:
    virtual void updateSources(const Shader* shader) const override;
    virtual void compile(const Shader* shader) const override;
};

} // namespace glow
