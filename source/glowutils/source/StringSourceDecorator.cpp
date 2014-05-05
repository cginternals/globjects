#include <glowutils/StringSourceDecorator.h>

#include <cassert>

namespace glowutils 
{

StringSourceDecorator::StringSourceDecorator(AbstractStringSource * source)
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

void StringSourceDecorator::notifyChanged(const glow::Changeable *)
{
	update();
    changed();
}

void StringSourceDecorator::update()
{
}

} // namespace glowutils
