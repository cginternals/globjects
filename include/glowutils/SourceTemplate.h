#pragma once

#include <string>
#include <map>

#include <glowutils/glowutils.h>
#include <glowutils/ShaderSourceDecorator.h>

namespace glow 
{

class GLOWUTILS_API SourceTemplate : public ShaderSourceDecorator
{
public:
	SourceTemplate(ShaderSource * source);
    virtual ~SourceTemplate();

    virtual const std::string & source() const;
    virtual void update() override;

	void replace(const std::string & orig, const std::string & str);
	void replace(const std::string & orig, int i);
protected:
    void modifySource();
protected:
    std::string m_modifiedSource;
	std::map<std::string, std::string> m_replacements;
};

} // namespace glow
