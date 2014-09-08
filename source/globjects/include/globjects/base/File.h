#pragma once

#include <string>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/globjects_api.h>

namespace glo
{

/** \brief String source associated to a file.
    
    The file path of a File can be queried using filePath(); To reload the contents
    from a file, use reload().

    \see StringSource
 */
class GLOBJECTS_API File : public glo::AbstractStringSource
{
public:
    File(const std::string & filePath);

    virtual std::string string() const override;
    virtual std::string shortInfo() const override;

    const std::string & filePath() const;

    void reload();

    static void reloadAll();
protected:
    std::string m_filePath;
    mutable std::string m_source;
    mutable bool m_valid;

    virtual ~File();

    void loadFileContent() const;
};

} // namespace glo
