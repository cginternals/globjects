#include "IncludeProcessor.h"

#include <sstream>
#include <algorithm>
#include <cctype>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/glow.h>
#include <glow/Version.h>
#include <glow/AbstractStringSource.h>
#include <glow/StaticStringSource.h>
#include <glow/CompositeStringSource.h>
#include <glow/NamedString.h>

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

    inline bool startsWith(const std::string& string, char firstChar)
    {
        return !string.empty() && string.front() == firstChar;
    }

    inline bool endsWith(const std::string& string, char firstChar)
    {
        return !string.empty() && string.back() == firstChar;
    }
}

namespace glow {

IncludeProcessor::IncludeProcessor()
{
}

IncludeProcessor::~IncludeProcessor()
{
}

AbstractStringSource* IncludeProcessor::resolveIncludes(const AbstractStringSource* source, const std::vector<std::string>& includePaths)
{
    IncludeProcessor processor;
    processor.m_includePaths = includePaths;

    return processor.processComposite(source);
}

CompositeStringSource* IncludeProcessor::processComposite(const AbstractStringSource* source)
{
    CompositeStringSource* composite = new CompositeStringSource();

    for (const AbstractStringSource* innerSource : source->flatten())
    {
        composite->appendSource(process(innerSource));
    }

    return composite;
}

CompositeStringSource* IncludeProcessor::process(const AbstractStringSource* source)
{
    CompositeStringSource* compositeSource = new CompositeStringSource();

    std::istringstream sourcestream(source->string());
    std::stringstream destinationstream;

    bool inMultiLineComment = false;

    do
    {
        std::string line;

        std::getline(sourcestream, line);

        std::string trimmedLine = trim(line);

        if (trimmedLine.size() > 0)
        {
            if (trimmedLine[0] == '#')
            {
                if (contains(trimmedLine, "/*"))
                {
                    inMultiLineComment = true;
                }

                if (contains(trimmedLine, "*/"))
                {
                    inMultiLineComment = false;
                }

                if (!inMultiLineComment && contains(trimmedLine, "extension"))
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
                else if (!inMultiLineComment && contains(trimmedLine, "include"))
                {
                    size_t leftBracketPosition = trimmedLine.find_first_of('<');
                    size_t rightBracketPosition = trimmedLine.find_last_of('>');
                    size_t leftQuotePosition = trimmedLine.find_first_of('"');
                    size_t rightQuotePosition = trimmedLine.find_last_of('"');

                    size_t leftDelimiter = std::string::npos;
                    size_t rightDelimiter = std::string::npos;

                    if (leftBracketPosition != std::string::npos && rightBracketPosition != std::string::npos)
                    {
                        leftDelimiter = leftBracketPosition;
                        rightDelimiter = rightBracketPosition;
                    }
                    else if (leftQuotePosition != std::string::npos && rightQuotePosition != std::string::npos && leftQuotePosition < rightQuotePosition)
                    {
                        leftDelimiter = leftQuotePosition;
                        rightDelimiter = rightQuotePosition;
                    }

                    if (leftDelimiter != std::string::npos && rightDelimiter != std::string::npos)
                    {
                        std::string include = trimmedLine.substr(leftDelimiter+1, rightDelimiter - leftDelimiter - 1);

                        if (include.size() == 0 || endsWith(include, '/'))
                        {
                            glow::warning() << "Malformed #include " << include;
                        }
                        else
                        {
                            if (m_includes.count(include) == 0)
                            {
                                m_includes.insert(include);
                                compositeSource->appendSource(new StaticStringSource(destinationstream.str()));

                                NamedString * namedString = nullptr;
                                if (startsWith(include, '/'))
                                {
                                    namedString = NamedString::obtain(include);
                                }
                                else
                                {
                                    for (const std::string& prefix : m_includePaths)
                                    {
                                        std::string fullPath = expandPath(include, prefix);
                                        namedString = NamedString::obtain(fullPath);
                                        if (namedString)
                                        {
                                            break;
                                        }
                                    }
                                }

                                if (namedString)
                                {
                                    compositeSource->appendSource(processComposite(namedString->stringSource()));
                                }
                                else
                                {
                                    glow::warning() << "Did not find include " << include;
                                }

                                destinationstream.str("");
                            }
                        }
                    }
                    else
                    {
                        glow::warning() << "Malformed #include " << trimmedLine;
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
        else
        {
            // empty line
            destinationstream << line << '\n';
        }
    }
    while (sourcestream.good());

    if (!destinationstream.str().empty())
    {
        compositeSource->appendSource(new StaticStringSource(destinationstream.str()));
    }

    return compositeSource;
}

std::string IncludeProcessor::expandPath(const std::string& include, const std::string includePath)
{
    return endsWith(includePath, '/') ? includePath + include : includePath + "/" + include;
}

} // namespace glow
