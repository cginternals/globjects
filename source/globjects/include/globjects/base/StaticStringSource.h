#pragma once

#include <string>

#include <globjects/globjects_api.h>
#include <globjects/base/AbstractStringSource.h>

namespace globjects 
{

/** \brief StaticStringSource is a StringSource using an std::string as source.

    The string can be queried with string().
    
    \see StringSource
 */
class GLOBJECTS_API StaticStringSource : public AbstractStringSource
{
public:
    StaticStringSource(const std::string & string);
    StaticStringSource(const char * data, size_t length);

    virtual std::string shortInfo() const override;
    virtual std::string string() const override;

    void setString(const std::string & string);

protected:
    std::string m_string;
};

} // namespace globjects
