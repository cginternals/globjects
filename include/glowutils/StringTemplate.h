#pragma once

#include <string>
#include <map>

#include <glowutils/glowutils.h>
#include <glowutils/StringSourceDecorator.h>

namespace glow 
{

class GLOWUTILS_API StringTemplate : public StringSourceDecorator
{
public:
    StringTemplate(StringSource * source);
    virtual ~StringTemplate();

    virtual const std::string & string() const override;
    virtual void update() override;

	void replace(const std::string & orig, const std::string & str);
    void replace(const std::string & orig, int i);
protected:
    std::string m_modifiedSource;
	std::map<std::string, std::string> m_replacements;
};

} // namespace glow
