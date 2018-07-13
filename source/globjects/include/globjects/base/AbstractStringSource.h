
#pragma once


#include <set>
#include <string>
#include <vector>

#include <globjects/globjects_api.h>


namespace globjects
{


class Shader;
class NamedString;


/** \brief Superclass for all types of static and dynamic strings, e.g. for the use as Shader code.
 *
 * The current string can be queried using string().
 *
 * \see Shader
 */
class GLOBJECTS_API AbstractStringSource
{
public:
    AbstractStringSource();
    virtual ~AbstractStringSource();

    virtual std::string string() const = 0;
    virtual std::vector<std::string> strings() const;

    std::vector<const AbstractStringSource*> flatten() const;
    virtual void flattenInto(std::vector<const AbstractStringSource*> & vector) const;

    virtual std::string shortInfo() const;

    void changed() const;

    void registerListener(AbstractStringSource * listener);
    void deregisterListener(AbstractStringSource * listener);

    void registerListener(Shader * listener);
    void deregisterListener(Shader * listener);

    void registerListener(NamedString * listener);
    void deregisterListener(NamedString * listener);

    void addSubject(AbstractStringSource * subject);
    virtual void removeSubject(AbstractStringSource * subject);

    virtual void notifyChanged(const AbstractStringSource * sender);

protected:
    std::set<AbstractStringSource *>    m_sourceListener;
    std::set<Shader *>                  m_shaderListener;
    std::set<NamedString *>             m_namedStringListener;
    std::set<AbstractStringSource*>     m_subjects;
};


} // namespace globjects
