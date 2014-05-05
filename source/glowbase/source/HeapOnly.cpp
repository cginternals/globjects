#include <glowbase/HeapOnly.h>

namespace glow {

HeapOnly::HeapOnly()
{
}

void HeapOnly::destroy() const
{
    delete this;
}

HeapOnly::~HeapOnly()
{
}

} // namespace glowbase
