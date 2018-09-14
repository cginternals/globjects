
#pragma once


#include <string>
#include <vector>

#include <globjects/globjects_api.h>

#include <globjects/base/Instantiator.h>
#include <globjects/base/AbstractStringSource.h>


namespace globjects
{


class GLOBJECTS_API CompositeStringSource : public AbstractStringSource, public Instantiator<CompositeStringSource>
{
public:
    CompositeStringSource();
    CompositeStringSource(const std::vector<AbstractStringSource *> & sources);

    virtual ~CompositeStringSource();

    void appendSource(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual std::vector<std::string> strings() const override;

    virtual void flattenInto(std::vector<const AbstractStringSource *> & vector) const override;

    void addSubject(AbstractStringSource * subject);
    void removeSubject(AbstractStringSource * subject);

    virtual std::string shortInfo() const override;
protected:

    virtual void notifyChanged(const AbstractStringSource * changeable) override;

    void update() const;

protected:
    std::vector<AbstractStringSource *> m_sources;
    
    mutable bool m_dirty;
    mutable std::vector<std::string> m_strings;
};


} // namespace globjects
