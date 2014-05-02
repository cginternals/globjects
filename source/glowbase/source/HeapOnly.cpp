#include <glowbase/HeapOnly.h>

namespace glowbase {

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
