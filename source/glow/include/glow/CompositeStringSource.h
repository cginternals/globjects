#pragma once

#include <string>
#include <vector>

#include <glow/glow_api.h>
#include <glow/ref_ptr.h>
#include <glow/AbstractStringSource.h>
#include <glow/ChangeListener.h>

namespace glow
{

class GLOW_API CompositeStringSource : public AbstractStringSource, protected ChangeListener
{
public:
    CompositeStringSource();
    CompositeStringSource(const std::vector<AbstractStringSource*> & sources);

    void appendSource(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual std::vector<std::string> strings() const override;
    virtual void flattenInto(std::vector<const AbstractStringSource*>& vector) const override;

    virtual std::string shortInfo() const override;
protected:
    virtual ~CompositeStringSource();
    virtual void notifyChanged(const Changeable * changeable) override;
    void update() const;
protected:
    std::vector<ref_ptr<AbstractStringSource>> m_sources;
    mutable bool m_dirty;
    mutable std::vector<std::string> m_strings;
};

} // namespace glow
