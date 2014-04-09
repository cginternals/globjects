#pragma once

#include <string>

#include <glow/glow_api.h>
#include <glow/AbstractStringSource.h>

namespace glow 
{

/** \brief StaticStringSource is a StringSource using an std::string as source.

    The string can be queried with string().
    
    \see StringSource
 */
class GLOW_API StaticStringSource : public AbstractStringSource
{
public:
    StaticStringSource(const std::string& string);
    StaticStringSource(const char * data, size_t length);

    virtual std::string shortInfo() const override;
    virtual std::string string() const override;

    void setString(const std::string& string);
protected:
    std::string m_string;
};

} // namespace glow
