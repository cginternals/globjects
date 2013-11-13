#include <sstream>
#include <cassert>

#include <glowutils/StringTemplate.h>

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

StringTemplate::StringTemplate(StringSource * source)
: StringSourceDecorator(source)
{
}

StringTemplate::~StringTemplate()
{
}

const std::string & StringTemplate::string() const
{
	return m_modifiedSource;
}

void StringTemplate::replace(
    const std::string & orig
,   const std::string & str)
{
	m_replacements[orig] = str;
}

void StringTemplate::replace(const std::string & orig, int i)
{
	std::stringstream ss;
	ss << i;
	replace(orig, ss.str());
}

void StringTemplate::update()
{
    m_modifiedSource = m_internal->string();

    for (const std::pair<std::string, std::string>& pair: m_replacements)
        replaceAll(m_modifiedSource, pair.first, pair.second);
}

} // namespace glow
