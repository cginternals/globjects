#pragma once

#include "AbstractObjectNameImplementation.h"

namespace globjects
{

class ObjectNameImplementation_KHR_debug : public AbstractObjectNameImplementation
{
public:
    ObjectNameImplementation_KHR_debug();
    virtual ~ObjectNameImplementation_KHR_debug();

    virtual std::string getLabel(const Object * object) const override;
    virtual std::string getLabel(const Sync * sync) const override;

    virtual bool hasLabel(const Object * object) const override;
    virtual bool hasLabel(const Sync * sync) const override;

    virtual void setLabel(const Object * object, const std::string & label) const override;
    virtual void setLabel(const Sync * sync, const std::string & label) const override;
protected:
    unsigned int m_maxLabelLength;
};

} // namespace globjects
