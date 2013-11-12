#pragma once

#include <string>
#include <vector>
#include <set>

#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>
#include <glowutils/ShaderSourceDecorator.h>

namespace glow 
{

class Shader;
class ShaderSource;

class GLOWUTILS_API CompoundSource : public ShaderSourceDecorator
{
public:
    CompoundSource(ShaderSource * source);
    virtual ~CompoundSource();

    virtual void update();

    std::set<Shader*> requiredShaders() const override;

    void resolveInclude(const std::string& include, Shader * shader);
    void propagateFilename(const std::string filename);
protected:
    std::string m_translatedSource;
};

} // namespace glow
