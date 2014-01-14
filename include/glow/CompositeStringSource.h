#pragma once

#include <string>
#include <vector>

#include <glow/glow.h>
#include <glow/ref_ptr.h>
#include <glow/StringSource.h>
#include <glow/ChangeListener.h>

namespace glow
{

class GLOW_API CompositeStringSource : public StringSource, protected ChangeListener
{
public:
    CompositeStringSource();
    CompositeStringSource(const std::vector<StringSource*> & sources);

    ~CompositeStringSource();

    void appendSource(StringSource * source);

    virtual std::string string() const override;
    virtual std::vector<std::string> strings() const override;
    virtual void flattenInto(std::vector<StringSource*>& vector) const override;

    virtual std::string shortInfo() const override;
protected:
    virtual void notifyChanged() override;
    void update() const;
protected:
    std::vector<ref_ptr<StringSource>> m_sources;
    mutable bool m_dirty;
    mutable std::vector<std::string> m_strings;
};

} // namespace glow
