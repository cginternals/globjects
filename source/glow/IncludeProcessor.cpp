#include "IncludeProcessor.h"

#include <sstream>
#include <algorithm>
#include <cctype>

#include <GL/glew.h>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/NamedStrings.h>
#include <glow/Version.h>

namespace {
    // From http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    inline std::string trim(const std::string &s)
    {
       auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
       auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
       return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
    }

    inline bool contains(const std::string& string, const std::string& search)
    {
        return string.find(search) != std::string::npos;
    }
}

namespace glow {

IncludeProcessor::IncludeProcessor()
{
}

IncludeProcessor::~IncludeProcessor()
{
}

std::string IncludeProcessor::resolveIncludes(const std::string& source)
{
    if (GLEW_ARB_shading_language_include && Version::current() >= Version(3, 2))
    {
        return source;
    }
    else
    {
        return IncludeProcessor().process(source);
    }
}

std::string IncludeProcessor::process(const std::string& source)
{
    std::istringstream sourcestream(source);
    std::stringstream destinationstream;

    do
    {
        std::string line;

        std::getline(sourcestream, line);

        std::string trimmedLine = trim(line);

        if (trimmedLine.size() >= 0)
        {
            if (trimmedLine[0] == '#')
            {
                if (contains(trimmedLine, "extension"))
                {
                    // #extension GL_ARB_shading_language_include : require
                    if (contains(trimmedLine, "GL_ARB_shading_language_include"))
                    {
                        // drop line
                    }
                    else
                    {
                        destinationstream << line << '\n';
                    }
                }
                else if (contains(trimmedLine, "include"))
                {
                    size_t leftBracketPosition = trimmedLine.find('<');
                    size_t rightBracketPosition = trimmedLine.find('>');

                    if (leftBracketPosition == std::string::npos || rightBracketPosition == std::string::npos)
                    {
                        glow::warning() << "Malformed #include";
                    }
                    else
                    {
                        std::string include = trimmedLine.substr(leftBracketPosition+1, rightBracketPosition - leftBracketPosition - 1);

                        if (include.size() == 0 || include[0] != '/')
                        {
                            glow::warning() << "Malformed #include";
                        }
                        else
                        {
                            if (m_includes.count(include) == 0)
                            {
                                m_includes.insert(include);
                                destinationstream << resolveIncludes(NamedStrings::namedString(include));
                            }
                        }
                    }
                }
                else
                {
                    // other macro
                    destinationstream << line << '\n';
                }
            }
            else
            {
                // normal line
                destinationstream << line << '\n';
            }
        }
    }
    while (sourcestream.good());

    return destinationstream.str();
}

} // namespace glow
