#include <sstream>
#include <cassert>

#include <glowutils/SourceTemplate.h>

namespace 
{

void replaceAll(
    std::string & subject
,   const std::string & search
,   const std::string & replace)
{
	size_t pos = 0;

	while((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

}

namespace glow 
{

SourceTemplate::SourceTemplate(ShaderSource * source)
: ShaderSourceDecorator(source)
{
}

SourceTemplate::~SourceTemplate()
{
}

const std::string & SourceTemplate::source() const
{
	return m_modifiedSource;
}

void SourceTemplate::replace(
    const std::string & orig
,   const std::string & str)
{
	m_replacements[orig] = str;
}

void SourceTemplate::replace(const std::string & orig, int i)
{
	std::stringstream ss;
	ss << i;
	replace(orig, ss.str());
}

void SourceTemplate::update()
{
	modifySource();
	changed();
}

void SourceTemplate::modifySource()
{
	m_modifiedSource = m_internal->source();

    for (const std::pair<std::string, std::string>& pair: m_replacements)
		replaceAll(m_modifiedSource, pair.first, pair.second);
}

} // namespace glow
