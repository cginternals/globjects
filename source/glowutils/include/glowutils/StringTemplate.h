#pragma once

#include <string>
#include <map>

#include <glowbase/CachedValue.h>

#include <glowutils/glowutils_api.h>
#include <glowutils/StringSourceDecorator.h>

namespace glowutils 
{

class GLOWUTILS_API StringTemplate : public StringSourceDecorator
{
public:
    StringTemplate(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual void update() override;

    void replace(const std::string & original, const std::string & str);
    void replace(const std::string & original, int i);

    void clearReplacements();

protected:
    glow::CachedValue<std::string> m_modifiedSource;
	std::map<std::string, std::string> m_replacements;

    virtual ~StringTemplate();

    void invalidate();
    std::string modifiedSource() const;
};

} // namespace glowutils
