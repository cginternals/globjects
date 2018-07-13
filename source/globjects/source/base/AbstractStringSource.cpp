
#include <globjects/base/AbstractStringSource.h>

#include <cassert>

#include <globjects/Shader.h>
#include <globjects/NamedString.h>


namespace globjects
{


AbstractStringSource::AbstractStringSource()
{
}

AbstractStringSource::~AbstractStringSource()
{
    while (!m_subjects.empty())
    {
        // calls removeSubject
        (*m_subjects.begin())->deregisterListener(this);
    }

    while (!m_sourceListener.empty())
    {
        // calls deregisterListener
        (*m_sourceListener.begin())->removeSubject(this);
    }

    while (!m_shaderListener.empty())
    {
        // calls deregisterListener
        (*m_shaderListener.begin())->removeSubject(this);
    }

    while (!m_namedStringListener.empty())
    {
        // calls deregisterListener
        (*m_namedStringListener.begin())->removeSubject(this);
    }
}

void AbstractStringSource::changed() const
{
    for (AbstractStringSource * listener: m_sourceListener)
    {
        listener->notifyChanged(this);
    }

    for (Shader * listener: m_shaderListener)
    {
        listener->notifyChanged(this);
    }

    for (NamedString * listener: m_namedStringListener)
    {
        listener->notifyChanged(this);
    }
}

void AbstractStringSource::registerListener(AbstractStringSource * listener)
{
    assert(listener != nullptr);

    m_sourceListener.insert(listener);
    listener->addSubject(this);
}

void AbstractStringSource::deregisterListener(AbstractStringSource * listener)
{
    assert(listener != nullptr);

    const auto it = m_sourceListener.find(listener);

    if (it == m_sourceListener.end())
    {
        return;
    }

    m_sourceListener.erase(it);
    listener->removeSubject(this);
}

void AbstractStringSource::registerListener(Shader * listener)
{
    assert(listener != nullptr);

    m_shaderListener.insert(listener);
    listener->addSubject(this);
}

void AbstractStringSource::deregisterListener(Shader * listener)
{
    assert(listener != nullptr);

    const auto it = m_shaderListener.find(listener);

    if (it == m_shaderListener.end())
    {
        return;
    }

    m_shaderListener.erase(it);
    listener->removeSubject(this);
}

void AbstractStringSource::registerListener(NamedString * listener)
{
    assert(listener != nullptr);

    m_namedStringListener.insert(listener);
    listener->addSubject(this);
}

void AbstractStringSource::deregisterListener(NamedString * listener)
{
    assert(listener != nullptr);

    const auto it = m_namedStringListener.find(listener);

    if (it == m_namedStringListener.end())
    {
        return;
    }

    m_namedStringListener.erase(it);
    listener->removeSubject(this);
}

void AbstractStringSource::addSubject(AbstractStringSource * subject)
{
    m_subjects.insert(subject);
}

void AbstractStringSource::removeSubject(AbstractStringSource * subject)
{
    assert(subject != nullptr);

    const auto it = m_subjects.find(subject);

    if (it == m_subjects.end())
    {
        return;
    }

    m_subjects.erase(it);
    subject->deregisterListener(this);
}

void AbstractStringSource::notifyChanged(const AbstractStringSource *)
{
}

std::vector<std::string> AbstractStringSource::strings() const
{
    std::vector<std::string> stringList;
    stringList.push_back(string());
    return stringList;
}

std::vector<const AbstractStringSource*> AbstractStringSource::flatten() const
{
    std::vector<const AbstractStringSource*> list;

    flattenInto(list);

    return list;
}

std::string AbstractStringSource::shortInfo() const
{
    return "";
}

void AbstractStringSource::flattenInto(std::vector<const AbstractStringSource*> & vector) const
{
    vector.push_back(this);
}


} // namespace globjects
