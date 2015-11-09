#include <globjects/base/StringTemplate.h>

#include <sstream>
#include <cassert>


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

namespace globjects
{

StringTemplate::StringTemplate(AbstractStringSource * source)
: StringSourceDecorator(source)
, m_modifiedSourceValid(false)
{
}

StringTemplate::~StringTemplate()
{
}

std::string StringTemplate::string() const
{
    if (!m_modifiedSourceValid)
    {
        m_modifiedSource = modifiedSource();
        m_modifiedSourceValid = true;
    }

    return m_modifiedSource;
}

void StringTemplate::clearReplacements()
{
    m_replacements.clear();
    invalidate();
}

void StringTemplate::replace(const std::string & original, const std::string & str)
{
    m_replacements[original] = str;
    invalidate();
}

void StringTemplate::replace(const std::string & original, const int i)
{
	std::stringstream ss;
	ss << i;
    replace(original, ss.str());
}

void StringTemplate::update()
{
    invalidate();
}

void StringTemplate::invalidate()
{
    m_modifiedSourceValid = false;
    changed();
}

std::string StringTemplate::modifiedSource() const
{
    std::string source = m_internal->string();

    for (const std::pair<std::string, std::string>& pair: m_replacements)
        replaceAll(source, pair.first, pair.second);

    return source;
}

} // namespace globjects
