#pragma once

#include <string>

#include <glow/glow.h>
#include <glow/StringSource.h>

namespace glow 
{

/** \brief String is a StringSource using an std::string as source.

    The string can be queried with string().
    
    \see StringSource
 */
class GLOW_API String : public StringSource
{
public:
    String(const std::string& string);

    virtual std::string string() const override;

    void setString(const std::string& string);
protected:
    std::string m_string;
};

} // namespace glow
