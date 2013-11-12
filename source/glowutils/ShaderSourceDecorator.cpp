#include <cassert>

#include <glowutils/ShaderSourceDecorator.h>

namespace glow 
{

ShaderSourceDecorator::ShaderSourceDecorator(ShaderSource * source)
: m_internal(source)
{
    assert(source != nullptr);

	m_internal->registerListener(this);

    update();
}

ShaderSourceDecorator::~ShaderSourceDecorator()
{
	m_internal->deregisterListener(this);
}

void ShaderSourceDecorator::notifyChanged()
{
	update();
    changed();
}

void ShaderSourceDecorator::update()
{
}

} // namespace glow
