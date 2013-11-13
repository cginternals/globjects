#include <cassert>

#include <glowutils/StringSourceDecorator.h>

namespace glow 
{

StringSourceDecorator::StringSourceDecorator(StringSource * source)
: m_internal(source)
{
    assert(source != nullptr);

	m_internal->registerListener(this);

    update();
}

StringSourceDecorator::~StringSourceDecorator()
{
	m_internal->deregisterListener(this);
}

void StringSourceDecorator::notifyChanged()
{
	update();
    changed();
}

void StringSourceDecorator::update()
{
}

} // namespace glow
