#include <common/StringSourceDecorator.h>

#include <cassert>

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

void StringSourceDecorator::notifyChanged(const glo::Changeable *)
{
	update();
    changed();
}

void StringSourceDecorator::update()
{
}
