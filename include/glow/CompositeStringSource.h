#pragma once

#include <string>
#include <vector>

#include <glow/glow.h>
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

    ~CompositeStringSource();

    void appendSource(AbstractStringSource * source);

    virtual std::string string() const override;
    virtual std::vector<std::string> strings() const override;
    virtual void flattenInto(std::vector<AbstractStringSource*>& vector) const override;

    virtual std::string shortInfo() const override;
protected:
    virtual void notifyChanged(Changeable * changeable) override;
    void update() const;
protected:
    std::vector<ref_ptr<AbstractStringSource>> m_sources;
    mutable bool m_dirty;
    mutable std::vector<std::string> m_strings;
};

} // namespace glow
