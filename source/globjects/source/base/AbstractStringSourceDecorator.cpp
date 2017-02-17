
#include <globjects/base/AbstractStringSourceDecorator.h>

#include <cassert>


namespace globjects
{


AbstractStringSourceDecorator::AbstractStringSourceDecorator(AbstractStringSource * source)
: m_internal(source)
{
    assert(source != nullptr);

    m_internal->registerListener(this);

    update();
}

AbstractStringSourceDecorator::~AbstractStringSourceDecorator()
{
    m_internal->deregisterListener(this);
}

void AbstractStringSourceDecorator::notifyChanged(const globjects::Changeable *)
{
    update();
    changed();
}

void AbstractStringSourceDecorator::update()
{
}


} // namespace globjects
