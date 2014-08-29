#pragma once

#include <globjects-base/Singleton.h>

#include "AbstractShadingLanguageIncludeImplementation.h"


namespace glo
{

class ShadingLanguageIncludeImplementation_ARB : public AbstractShadingLanguageIncludeImplementation
    , public Singleton<ShadingLanguageIncludeImplementation_ARB>
{
public:
    virtual void updateSources(const Shader * shader) const override;
    virtual void compile(const Shader * shader) const override;
};

} // namespace glo
