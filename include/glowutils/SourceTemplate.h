#pragma once

#include <string>
#include <map>

#include <glow/ShaderSource.h>
#include <glow/ChangeListener.h>

#include <glowutils/glowutils.h>

namespace glow 
{

class GLOWUTILS_API SourceTemplate : public glow::ShaderSource, protected glow::ChangeListener
{
public:
	SourceTemplate(glow::ShaderSource* source);
	~SourceTemplate();

	virtual const std::string& source();

	void replace(const std::string& orig, const std::string& str);
	void replace(const std::string& orig, int i);
	void update();

protected:
    virtual void notifyChanged();

    void modifySource();

protected:
	glow::ref_ptr<glow::ShaderSource> m_internal;
	std::string m_modifiedSource;
	std::map<std::string, std::string> m_replacements;
};

} // namespace glow
