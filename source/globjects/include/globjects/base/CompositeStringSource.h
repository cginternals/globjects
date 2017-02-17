
#pragma once

#include <string>
#include <vector>

#include <globjects/globjects_api.h>

#include <globjects/base/Instantiator.h>
#include <globjects/base/ChangeListener.h>
#include <globjects/base/AbstractStringSource.h>


namespace globjects
{


class GLOBJECTS_API CompositeStringSource : public AbstractStringSource, protected ChangeListener, public Instantiator<CompositeStringSource>
{
public:
    CompositeStringSource();
    CompositeStringSource(const std::vector<AbstractStringSource *> & sources);

    virtual ~CompositeStringSource();

    void appendSource(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual std::vector<std::string> strings() const override;

    virtual void flattenInto(std::vector<const AbstractStringSource *> & vector) const override;

    virtual std::string shortInfo() const override;
protected:

    virtual void notifyChanged(const Changeable * changeable) override;

    void update() const;

protected:
    std::vector<AbstractStringSource *> m_sources;
    
    mutable bool m_dirty;
    mutable std::vector<std::string> m_strings;
};


} // namespace globjects
