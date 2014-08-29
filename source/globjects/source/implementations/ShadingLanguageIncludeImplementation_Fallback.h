#pragma once

#include <globjects-base/Singleton.h>

#include "AbstractShadingLanguageIncludeImplementation.h"


namespace glo
{

class ShadingLanguageIncludeImplementation_Fallback : public AbstractShadingLanguageIncludeImplementation
    , public Singleton<ShadingLanguageIncludeImplementation_Fallback>
{
public:
    virtual void updateSources(const Shader * shader) const override;
    virtual void compile(const Shader * shader) const override;
};

} // namespace glo
